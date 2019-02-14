#include "trainwidget.h"
#include "ui_trainwidget.h"

#include <QMessageBox>

#include <algorithm>

TrainWidget::TrainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainWidget)
{
    ui->setupUi(this);

    setupLayout();
}

TrainWidget::~TrainWidget()
{
    delete ui;
}

void TrainWidget::onTrainKanjiSet(std::vector<kcomp> newTraining)
{
    trainKanji = std::move(newTraining);

    // to enable "go back"
    resKanji = trainKanji;
    history.clear();
    std::transform(trainKanji.begin(), trainKanji.end(),
                   std::inserter(validId, validId.begin()),
                   [](const kcomp &kc){ return kc.get_id(); });

    newCycle();
}

void TrainWidget::onFlipClicked()
{
    flipButton->hide();

    kanjiReading->show();
    kanjiMeaning->show();
    feedbackSplitter->show();

    flipped = true;
}

void TrainWidget::onResponseSelected(FlipResponse fr)
{
    // skip if unsure
    if (fr != FlipResponse::maybe) {

        // train succeeded
        auto trainedIt = std::find_if(resKanji.begin(), resKanji.end(),
                                 [=](const kcomp &kc){
            return kc.get_id() == currKanji->get_id();
        });

        trainedIt->repeat(fr == FlipResponse::yes);

        validId.erase(currKanji->get_id());
    }

    history.push_back(currKanji->get_id());

    currKanji = std::find_if(currKanji + 1, trainKanji.end(), [&](const kcomp &kc) {
        return validId.find(kc.get_id()) != validId.end();
    });

    // start a new training cycle (some might have been skipped)
    if (currKanji == trainKanji.end()) {
        newCycle();
        return;
    }

    // update page
    updateKanjiLabels();
    flipBack();
}

void TrainWidget::onTrainingEndClicked()
{
    QMessageBox endBox;
    endBox.setText("Training exit");
    endBox.setInformativeText("Do you want to save training progress?");
    endBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    endBox.setDefaultButton(QMessageBox::Cancel);

    auto res = endBox.exec();
    switch (res) {
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Yes:
            emit trainingEnded(trainKanji);
            return;
        case QMessageBox::No:
            emit trainingDiscarded();
            return;
    }
}

void TrainWidget::onBackButtonClicked()
{
    if (history.size() == 0) {
        return;
    }

    if (flipped) {
        flipBack();
        return;
    }

    // revert training
    auto trainedIt = std::find_if(resKanji.begin(), resKanji.end(),
                             [=](const kcomp &kc){
        return kc.get_id() == currKanji->get_id();
    });
    *trainedIt = *currKanji;

    kcomp::kanji_id bef = history[history.size() - 1];
    history.pop_back();

    // find the previous kanji
    currKanji = std::find_if(trainKanji.begin(), trainKanji.end(),
                             [=](const kcomp &kc){
        return kc.get_id() == bef;
    });

    updateKanjiLabels();
}

void TrainWidget::showEvent(QShowEvent *e)
{
    emit customMenuShown(menu);
    emit trainingStarted();
}

void TrainWidget::hideEvent(QHideEvent *e)
{
    emit customMenuHidden();

    // could be emitted (save training,...)
    emit trainingDiscarded();
}

void TrainWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupButtons();
    setupTrainPage();
    setupFlippedPage();

    setLayout(l);
}

void TrainWidget::setupButtons()
{
    // TODO hide menu splitter in MainWidget!

    menu = new QSplitter(Qt::Orientation::Horizontal);


    // back
    QPushButton *backButton = new QPushButton("Back");
    backButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    connect(backButton, &QPushButton::clicked,
            this, &TrainWidget::onBackButtonClicked);

    // end
    QPushButton *endButton = new QPushButton("End training");
    endButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    connect(endButton, &QPushButton::clicked,
            this, &TrainWidget::onTrainingEndClicked);

    menu->addWidget(backButton);
    menu->addWidget(endButton);

    menu->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void TrainWidget::setupTrainPage()
{
    kanjiText = new QLabel();
    kanjiText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    connect(this, &TrainWidget::kanjiChanged,
            kanjiText, &QLabel::setText);

    flipButton = new QPushButton("Flip");
    connect(flipButton, &QPushButton::clicked,
            this, &TrainWidget::onFlipClicked);

    // add widgets
    l->addWidget(kanjiText);
    l->addWidget(flipButton);
}

void TrainWidget::setupFlippedPage()
{
    // kanji reading and meaning
    kanjiReading = new QLabel();
    connect(this, &TrainWidget::kanjiReadingChanged,
            kanjiReading, &QLabel::setText);

    kanjiMeaning = new QLabel();
    connect(this, &TrainWidget::kanjiMeaningChanged,
            kanjiMeaning, &QLabel::setText);

    // training buttons
    feedbackSplitter = new QSplitter();

    QPushButton *yesButton = new QPushButton("Yes");
    connect(yesButton, &QPushButton::clicked,
            this, [=](){
        this->onResponseSelected(FlipResponse::yes);
    });

    QPushButton *maybeButton = new QPushButton("Not sure");
    connect(maybeButton, &QPushButton::clicked,
            this, [=](){
        this->onResponseSelected(FlipResponse::maybe);
    });

    QPushButton *noButton = new QPushButton("No");
    connect(noButton, &QPushButton::clicked,
            this, [=](){
        this->onResponseSelected(FlipResponse::no);
    });

    feedbackSplitter->addWidget(yesButton);
    feedbackSplitter->addWidget(maybeButton);
    feedbackSplitter->addWidget(noButton);

    l->addWidget(kanjiReading);
    l->addWidget(kanjiMeaning);
    l->addWidget(feedbackSplitter);

    kanjiReading->hide();
    kanjiMeaning->hide();
    feedbackSplitter->hide();
}

void TrainWidget::updateKanjiLabels()
{
    emit kanjiChanged(QString::fromStdWString(currKanji->get_kanji()));
    emit kanjiMeaningChanged(QString::fromStdWString(currKanji->meaning));
    emit kanjiReadingChanged(QString::fromStdWString(currKanji->reading));
}

void TrainWidget::newCycle()
{
    currKanji = std::find_if(trainKanji.begin(), trainKanji.end(),
                             [&](const kcomp &kc) {
        return validId.find(kc.get_id()) != validId.end();
    });

    // all trained, return modified kanji
    if (currKanji == trainKanji.end()) {
        emit trainingEnded(trainKanji);
        return;
    }

    updateKanjiLabels();
    if (flipped) {
        flipBack();
    }
}

void TrainWidget::flipBack()
{
    flipButton->show();

    kanjiReading->hide();
    kanjiMeaning->hide();
    feedbackSplitter->hide();

    flipped = false;
}

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


    if (trainKanji.size() == 0) {
        QMessageBox noTrainBox;
        noTrainBox.setText("No training kanji available.");
        noTrainBox.setInformativeText("Wait until there are kanji available for the training.");
        noTrainBox.exec();

        emit trainingDiscarded();
        return;
    }

    updateRemainingLabel();
    newCycle();
}

void TrainWidget::onFlipClicked()
{
    flipped = true;

    flipButton->hide();

    updateKanjiLabels();
    feedbackSplitter->show();
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

        // remaining count changed
        updateRemainingLabel();
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
            emit trainingEnded(resKanji);
            return;
        case QMessageBox::No:
            emit trainingDiscarded();
            return;
    }
}

void TrainWidget::onBackButtonClicked()
{
    if (flipped) {
        flipBack();
        return;
    }

    if (history.size() == 0) {
        return;
    }

    kcomp::kanji_id bef = history[history.size() - 1];
    history.pop_back();

    // find the previous kanji
    currKanji = std::find_if(trainKanji.begin(), trainKanji.end(),
                             [=](const kcomp &kc){
        return kc.get_id() == bef;
    });

    // revert training if necessary
    auto trainedIt = std::find_if(resKanji.begin(), resKanji.end(),
                             [=](const kcomp &kc){
        return kc.get_id() == currKanji->get_id();
    });

    if (trainedIt->get_level() != currKanji->get_level()) {
        validId.insert(currKanji->get_id());
        updateRemainingLabel();
    }

    // revert training in result
    *trainedIt = *currKanji;

    updateKanjiLabels();
}

void TrainWidget::showEvent(QShowEvent *e)
{
    emit customMenuShown(menu);
}

void TrainWidget::hideEvent(QHideEvent *e)
{
    emit customMenuHidden();
}

void TrainWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupMenuButtons();
    setupTrainPage();
    setupFlippedPage();
    setupButtons();

    setLayout(l);
}

void TrainWidget::setupMenuButtons()
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

    // this is on both flip and train
    QLabel *remainingLabel = new QLabel();
    connect(this, &TrainWidget::kanjiRemainingChanged,
            remainingLabel, &QLabel::setText);


    l->addWidget(remainingLabel);
}

void TrainWidget::setupTrainPage()
{
    kanjiText = new QLabel();
    kanjiText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    kanjiText->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    auto font = kanjiText->font();
    font.setPixelSize(48);
    kanjiText->setFont(font);

    connect(this, &TrainWidget::kanjiChanged,
            kanjiText, &QLabel::setText);

    // add widgets
    l->addWidget(kanjiText);
}

void TrainWidget::setupFlippedPage()
{
    // kanji reading and meaning
    kanjiReading = new QLabel();
    kanjiReading->setAlignment(Qt::AlignHCenter);
    connect(this, &TrainWidget::kanjiReadingChanged,
            kanjiReading, &QLabel::setText);

    auto rFont = kanjiReading->font();
    rFont.setPixelSize(14);
    kanjiReading->setFont(rFont);

    kanjiMeaning = new QLabel();
    kanjiMeaning->setAlignment(Qt::AlignHCenter);
    connect(this, &TrainWidget::kanjiMeaningChanged,
            kanjiMeaning, &QLabel::setText);

    auto mFont = kanjiMeaning->font();
    mFont.setPixelSize(14);
    kanjiMeaning->setFont(mFont);

    l->addWidget(kanjiReading);
    l->addWidget(kanjiMeaning);
}

void TrainWidget::setupButtons()
{
    // flip
    flipButton = new QPushButton("Flip");
    connect(flipButton, &QPushButton::clicked,
            this, &TrainWidget::onFlipClicked);

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

    l->addWidget(feedbackSplitter);
    l->addWidget(flipButton);

    feedbackSplitter->hide();
}

void TrainWidget::updateRemainingLabel()
{
    auto totalCount = trainKanji.size();
    auto remCount = totalCount - validId.size();

    QString remaining = QString("%1/%2").arg(remCount).arg(totalCount);

    emit kanjiRemainingChanged(remaining);
}

void TrainWidget::updateKanjiLabels()
{
    emit kanjiChanged(QString::fromStdWString(currKanji->get_kanji()));

    if (flipped) {
        emit kanjiMeaningChanged(QString::fromStdWString(currKanji->meaning));
        emit kanjiReadingChanged(QString::fromStdWString(currKanji->reading));
    }
    else {
        emit kanjiMeaningChanged("");
        emit kanjiReadingChanged("");
    }
}
void TrainWidget::newCycle()
{
    currKanji = std::find_if(trainKanji.begin(), trainKanji.end(),
                             [&](const kcomp &kc) {
        return validId.find(kc.get_id()) != validId.end();
    });

    // all trained, return modified kanji
    if (currKanji == trainKanji.end()) {
        emit trainingEnded(resKanji);
        return;
    }

    updateKanjiLabels();
    if (flipped) {
        flipBack();
    }
}

void TrainWidget::flipBack()
{
    flipped = false;
    flipButton->show();

    updateKanjiLabels();
    feedbackSplitter->hide();
}

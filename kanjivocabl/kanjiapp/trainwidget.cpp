#include "trainwidget.h"
#include "ui_trainwidget.h"

#include <QMessageBox>

#include <algorithm>

TrainWidget::TrainWidget(Settings s,QWidget *parent) :
    QWidget(parent),
    s(std::move(s)),
    ui(new Ui::TrainWidget)
{
    ui->setupUi(this);

    setupLayout();
}

TrainWidget::~TrainWidget()
{
    delete ui;
}


// sets a new training set
void TrainWidget::onTrainKanjiSet(QVector<kcomp> newTraining)
{
    trainKanji = std::move(newTraining);

    /* "go back"  setup*/

    // trainKanji is not modified, resKanji is a copy for changes
    resKanji = trainKanji;

    //stack of trained ids
    history.clear();
    validId.clear();

    // initially, all kanji ids are valid (not trained with "yes" or "no")
    std::transform(trainKanji.begin(), trainKanji.end(),
                   std::inserter(validId, validId.begin()),
                   [](const kcomp &kc){
        return kc.get_id();
    });


    // training set must be nonempty
    if (trainKanji.size() == 0) {
        QMessageBox noTrainBox;
        noTrainBox.setText("No training kanji available.");
        noTrainBox.setInformativeText("Wait until there are kanji available for the training.");
        noTrainBox.exec();

        emit trainingDiscarded();
        return;
    }

    // start the training
    updateRemainingLabel();
    newCycle();
}


void TrainWidget::onSettingsLoaded(Settings s)
{
    this->s = std::move(s);
}



// flips the flashcard
void TrainWidget::onFlipClicked()
{
    flipped = true;

    // hide flip button, show training buttons
    flipButton->hide();

    updateKanjiLabels();
    feedbackSplitter->show();
}


// collects feedback from the user and modifies the compound
void TrainWidget::onResponseSelected(FlipResponse fr)
{
    // skip if unsure
    if (fr != FlipResponse::maybe) {

        // kanji compound is being repeated
        auto trainedIt = std::find_if(resKanji.begin(), resKanji.end(),
                                 [=](const kcomp &kc){
            return kc.get_id() == currKanji->get_id();
        });

        // "yes" or "no"
        trainedIt->repeat(fr == FlipResponse::yes, s.maxLevel, s.multiplier);

        // the compound is repeated only once in a training
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

    // update page to show the next compound
    updateKanjiLabels();
    flipBack();
}


// ends the training and offers to save training progress
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


// goes back in history or flips the flashcard back
void TrainWidget::onBackButtonClicked()
{
    // flip back, do not change current kanji
    if (flipped) {
        flipBack();
        return;
    }

    // the first kanji
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

    // training is reverted in the result kanji vector
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


// sets up the layout
void TrainWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupMenuButtons();
    setupTrainPage();
    setupFlippedPage();
    setupButtons();

    setLayout(l);
}


// sets up the custom menu
void TrainWidget::setupMenuButtons()
{
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


// sets up the train page (not flipped)
void TrainWidget::setupTrainPage()
{
    // these labels are both on train and flip page in this layout

    // kanji remaining
    QLabel *remainingLabel = new QLabel();
    connect(this, &TrainWidget::kanjiRemainingChanged,
            remainingLabel, &QLabel::setText);

    // kanji characters
    kanjiText = new QLabel();
    kanjiText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    kanjiText->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    auto font = kanjiText->font();
    font.setPixelSize(48);
    kanjiText->setFont(font);

    connect(this, &TrainWidget::kanjiChanged,
            kanjiText, &QLabel::setText);

    // add widgets
    l->addWidget(remainingLabel);
    l->addWidget(kanjiText);
}


// sets up the flipped page
void TrainWidget::setupFlippedPage()
{
    // kanji reading
    kanjiReading = new QLabel();
    kanjiReading->setAlignment(Qt::AlignHCenter);
    connect(this, &TrainWidget::kanjiReadingChanged,
            kanjiReading, &QLabel::setText);

    auto rFont = kanjiReading->font();
    rFont.setPixelSize(14);
    kanjiReading->setFont(rFont);

    // kanji meaning
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


// sets up buttons for both pages
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


// updates label which shows the number of remaining kanji
void TrainWidget::updateRemainingLabel()
{
    auto totalCount = trainKanji.size();
    auto remCount = totalCount - validId.size();

    QString remaining = QString("%1/%2").arg(remCount).arg(totalCount);

    emit kanjiRemainingChanged(remaining);
}


// updates displayed kanji data
void TrainWidget::updateKanjiLabels()
{
    emit kanjiChanged(QString::fromStdWString(currKanji->get_kanji()));

    // do not show all labels if the flascard is not flipped
    if (flipped) {
        emit kanjiMeaningChanged(QString::fromStdWString(currKanji->meaning));
        emit kanjiReadingChanged(QString::fromStdWString(currKanji->reading));
    }
    else {
        emit kanjiMeaningChanged("");
        emit kanjiReadingChanged("");
    }
}


// starts a new training cycle
void TrainWidget::newCycle()
{
    // find first kanji which was not repeated in a previous cycle
    currKanji = std::find_if(trainKanji.begin(), trainKanji.end(),
                             [&](const kcomp &kc) {
        return validId.find(kc.get_id()) != validId.end();
    });

    // everything has been trained, return modified kanji
    if (currKanji == trainKanji.end()) {
        emit trainingEnded(resKanji);
        return;
    }

    updateKanjiLabels();
    if (flipped) {
        flipBack();
    }
}


// flip the flashcard back
void TrainWidget::flipBack()
{
    flipped = false;
    flipButton->show();

    updateKanjiLabels();
    feedbackSplitter->hide();
}

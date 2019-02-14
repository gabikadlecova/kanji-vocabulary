#include "trainwidget.h"
#include "ui_trainwidget.h"

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
    history.clear();
    std::transform(trainKanji.begin(), trainKanji.end(), std::back_inserter(validId),
                   [](const kcomp &kc){ return kc.get_id(); });
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
    if (fr == FlipResponse::maybe) {
        return;
    }

    // train succeeded
    currKanji->repeat(fr == FlipResponse::yes);
    history.push_back(currKanji->get_id());

    // find next id
    auto idIt = std::find_if(validId.begin(), validId.end(),
                 [=](kcomp::kanji_id id) {
        return currKanji->get_id() == id;
    });

    bool end = idIt + 1 == validId.end();

    // start a new train cycle or finish training
    if (end) {
        validId.erase(idIt);

        newCycle();
        return;
    }

    // next kanji compound
    kcomp::kanji_id nextId = *(idIt + 1);
    currKanji = std::find_if(currKanji, trainKanji.end(), [=](const kcomp &kc) {
        return kc.get_id() == nextId;
    });

    // update page
    updateKanjiLabels();
    flipBack();
}

void TrainWidget::showEvent(QShowEvent *e)
{
    emit customMenuShown(menu);
    emit trainingStarted();
}

void TrainWidget::hideEvent(QHideEvent *e)
{
    emit customMenuHidden();
    emit trainingDiscarded();
}

void TrainWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupButtons();
    setupTrainPage();

    setLayout(l);
}

void TrainWidget::setupButtons()
{
    // TODO hide menu splitter in MainWidget!

    menu = new QSplitter(Qt::Orientation::Horizontal);


    // back
    QPushButton *backButton = new QPushButton("Back");
    backButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // end
    QPushButton *endButton = new QPushButton("End training");
    endButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    connect(endButton, &QPushButton::clicked,
            this, &TrainWidget::trainingEnded);

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

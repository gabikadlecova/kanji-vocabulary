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
    validId.erase(std::remove_if(validId.begin(), validId.end(),
                                 [=](kcomp::kanji_id id) {
                      return currKanji->get_id() == id;
                  }),
                  validId.end());
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
    kanjiText = new QLabel("todo");

    kanjiText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    l->addWidget(kanjiText);

    flipButton = new QPushButton("Flip");
    connect(flipButton, &QPushButton::clicked,
            this, &TrainWidget::onFlipClicked);

    l->addWidget(flipButton);

    // hidden buttons
    feedbackSplitter = new QSplitter();
    feedbackSplitter->hide();

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
}

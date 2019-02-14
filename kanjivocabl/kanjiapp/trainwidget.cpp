#include "trainwidget.h"
#include "ui_trainwidget.h"

#include <QPushButton>

TrainWidget::TrainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainWidget)
{
    ui->setupUi(this);

    pageId = -1;
    setupLayout();
}

TrainWidget::~TrainWidget()
{
    delete ui;
}

void TrainWidget::onTrainKanjiSet(std::vector<kcomp> newTraining)
{
    trainKanji = std::move(newTraining);
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
    l->addWidget(menu);
}

void TrainWidget::setupTrainPage()
{
    kanjiText = new QLabel("todo");

    kanjiText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    l->addWidget(kanjiText);
}

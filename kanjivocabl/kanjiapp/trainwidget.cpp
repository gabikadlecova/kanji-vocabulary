#include "trainwidget.h"
#include "ui_trainwidget.h"

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

void TrainWidget::setTrainKanji(std::vector<kcomp> newTraining)
{
    trainKanji = std::move(newTraining);
}

void TrainWidget::setupLayout()
{
    l = new QGridLayout();

    setupButtons();
    setupTrainPage();
}

void TrainWidget::setupButtons()
{
    // TODO hide menu splitter in MainWidget!
}

void TrainWidget::setupTrainPage()
{

}

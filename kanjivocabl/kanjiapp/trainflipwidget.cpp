#include "trainflipwidget.h"
#include "ui_trainflipwidget.h"

TrainFlipWidget::TrainFlipWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainFlipWidget)
{
    ui->setupUi(this);
}

TrainFlipWidget::~TrainFlipWidget()
{
    delete ui;
}

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

void TrainFlipWidget::on_ok_button_clicked()
{

}

void TrainFlipWidget::on_maybe_button_clicked()
{

}

void TrainFlipWidget::on_no_button_clicked()
{

}

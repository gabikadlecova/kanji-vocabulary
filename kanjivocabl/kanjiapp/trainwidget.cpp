#include "trainwidget.h"
#include "ui_trainwidget.h"

TrainWidget::TrainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainWidget)
{
    ui->setupUi(this);
}

TrainWidget::~TrainWidget()
{
    delete ui;
}

void TrainWidget::on_flip_button_clicked()
{

}

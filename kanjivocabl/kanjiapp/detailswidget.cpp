#include "detailswidget.h"
#include "ui_detailswidget.h"

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailsWidget)
{
    ui->setupUi(this);
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}

void DetailsWidget::on_edit_button_clicked()
{

}

void DetailsWidget::on_reset_button_clicked()
{

}

#include "kanjiwidget.h"
#include "ui_kanjiwidget.h"

#include "detailswidget.h"

#include <QStackedLayout>

KanjiWidget::KanjiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiWidget)
{
    ui->setupUi(this);
}

KanjiWidget::~KanjiWidget()
{
    delete ui;
}

void KanjiWidget::on_train_button_clicked()
{

}

void KanjiWidget::on_list_button_clicked()
{

}

void KanjiWidget::on_pushButton_clicked()
{

}

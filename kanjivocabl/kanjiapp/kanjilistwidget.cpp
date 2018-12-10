#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

KanjiListWidget::KanjiListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiListWidget)
{
    ui->setupUi(this);
}

KanjiListWidget::~KanjiListWidget()
{
    delete ui;
}

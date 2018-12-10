#include "stackitembase.h"
#include "ui_stackitembase.h"

StackItemBase::StackItemBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StackItemBase)
{
    ui->setupUi(this);
}

StackItemBase::~StackItemBase()
{
    delete ui;
}

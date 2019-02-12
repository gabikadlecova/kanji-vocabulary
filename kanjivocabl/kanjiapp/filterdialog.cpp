#include "filterdialog.h"
#include "ui_filterdialog.h"


FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    setupLayout();
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

// TODO QMap for ComboBox

void FilterDialog::setupLayout()
{
    l = new QGridLayout();

    setupFilter();
    setupButtons();

    setLayout(l);
}

void FilterDialog::setupFilter()
{
    cb = new QComboBox();
    // enum (and QMap) setup, connects

    filterVal = new QLineEdit();
    // connects

    l->addWidget(cb, 0, 0);
    l->addWidget(filterVal, 0, 1);
}

void FilterDialog::setupButtons()
{
    box = new QDialogButtonBox(QDialogButtonBox::Reset | QDialogButtonBox::Ok |
                               QDialogButtonBox::Cancel);

    // connects

    l->addWidget(box, 1, 0, 2, 0);
}

void FilterDialog::onFilterClicked()
{
    // TODO
}

void FilterDialog::onResetClicked()
{
    // TODO
}

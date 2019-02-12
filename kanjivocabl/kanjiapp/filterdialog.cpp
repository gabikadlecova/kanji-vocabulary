#include "filterdialog.h"
#include "ui_filterdialog.h"

#include <QPushButton>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    prevMode(FilterMode::none),
    prevString(""),
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

    modeMap.insert("None", FilterMode::none);
    modeMap.insert("By kanji", FilterMode::byKanji);
    modeMap.insert("By reading", FilterMode::byReading);
    modeMap.insert("By meaning", FilterMode::byMeaning);

    cb->addItems(QStringList(modeMap.keys()));

    filterVal = new QLineEdit();

    l->addWidget(cb, 0, 0);
    l->addWidget(filterVal, 0, 1);
}

void FilterDialog::setupButtons()
{
    box = new QDialogButtonBox(QDialogButtonBox::Reset | QDialogButtonBox::Ok |
                               QDialogButtonBox::Cancel);

    // connect - ok
    connect(box, &QDialogButtonBox::accepted,
            this, &FilterDialog::onFilterClicked);

    // connect - reset
    QPushButton *resetButton = box->button(QDialogButtonBox::Reset);
    connect(resetButton, &QPushButton::clicked,
            this, &FilterDialog::onResetClicked);


    l->addWidget(box, 1, 0, 2, 0);
}

void FilterDialog::onFilterClicked()
{
    // filter only if necessary
    auto fm = modeMap.value(cb->currentText());
    if (fm == prevMode && filterVal->text() == prevString) {
        return;
    }

    prevMode = fm;
    prevString = filterVal->text();

    emit filterConfirmed(fm, filterVal->text());
}

void FilterDialog::onResetClicked()
{
    filterVal->setText("");
    cb->setCurrentIndex(0);
}

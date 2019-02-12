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

    setModal(true);
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

    setFixedSize(width(), -1);

    setupFilter();
    setupButtons();

    setLayout(l);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    adjustSize();
}

void FilterDialog::setupFilter()
{
    cb = new QComboBox();

    // TODO to constants !!!
    modeMap.insert("None", FilterMode::none);
    modeMap.insert("By kanji", FilterMode::byKanji);
    modeMap.insert("By reading", FilterMode::byReading);
    modeMap.insert("By meaning", FilterMode::byMeaning);

    // we want to preserve order
    cb->addItems(QStringList{"None", "By kanji", "By reading", "By meaning"});

    filterVal = new QLineEdit();
    filterVal->setEnabled(false);

    connect(cb, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        emit enableFilterVal(text != "None");
    });

    connect(this, &FilterDialog::enableFilterVal,
            filterVal, &QLineEdit::setEnabled);


    l->addWidget(cb, 0, 0);
    l->addWidget(filterVal, 0, 1);
}

void FilterDialog::setupButtons()
{
    box = new QDialogButtonBox(QDialogButtonBox::Reset |
                               QDialogButtonBox::Cancel);
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // connect - reset
    QPushButton *resetButton = box->button(QDialogButtonBox::Reset);
    connect(resetButton, &QPushButton::clicked,
            this, &FilterDialog::onResetClicked);

    // connect - apply
    QPushButton *filterButton = new QPushButton("Apply");
    connect(filterButton, &QPushButton::clicked,
            this, &FilterDialog::onFilterClicked);

    box->addButton(filterButton, QDialogButtonBox::ButtonRole::ApplyRole);

    // connect - confirm
    QPushButton *confirmButton = new QPushButton("Confirm");
    connect(confirmButton, &QPushButton::clicked,
            this, &FilterDialog::accept);
    connect(confirmButton, &QPushButton::clicked,
            this, &FilterDialog::onFilterClicked);

    box->addButton(confirmButton, QDialogButtonBox::ButtonRole::AcceptRole);

    // connect - cancel
    QPushButton *cancelButton = box->button(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked,
            this, &FilterDialog::reject);

    l->addWidget(box, 1, 0, 2, 0);
}

void FilterDialog::onFilterClicked()
{
    // filter only if necessary
    auto fm = modeMap[cb->currentText()];
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

    if (prevMode == FilterMode::none) {
        return;
    }

    prevMode = FilterMode::none;
    prevString = "";

    emit filterConfirmed(FilterMode::none, "");
}

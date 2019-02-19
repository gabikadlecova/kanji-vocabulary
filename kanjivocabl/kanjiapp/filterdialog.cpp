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


// confirms the filter
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


// resets the filter
void FilterDialog::onResetClicked()
{
    filterVal->setText("");
    cb->setCurrentIndex(0);

    // reset only if necessary
    if (prevMode == FilterMode::none) {
        return;
    }

    prevMode = FilterMode::none;
    prevString = "";

    // signalizes filter reset
    emit filterConfirmed(FilterMode::none, "");
}


// sets up the layout
void FilterDialog::setupLayout()
{
    l = new QGridLayout();

    // vertical size should as low as possible
    setFixedSize(width(), 0);

    setupFilter();
    setupButtons();

    setLayout(l);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    adjustSize();
}


// sets up the filter contents
void FilterDialog::setupFilter()
{
    cb = new QComboBox();

    const QString none = "None";
    const QString byKanji = "By kanji";
    const QString byReading = "By reading";
    const QString byMeaning = "By meaning";

    modeMap.insert(none, FilterMode::none);
    modeMap.insert(byKanji, FilterMode::byKanji);
    modeMap.insert(byReading, FilterMode::byReading);
    modeMap.insert(byMeaning, FilterMode::byMeaning);

    // we want to preserve order (not ordered alphabetically as in the map)
    cb->addItems(QStringList{none, byKanji, byReading, byMeaning});

    // filter value is not enabled for "none"
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


// sets up the dialog buttons
void FilterDialog::setupButtons()
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Reset |
                               QDialogButtonBox::Cancel);
    buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // connect - reset
    QPushButton *resetButton = buttonBox->button(QDialogButtonBox::Reset);
    connect(resetButton, &QPushButton::clicked,
            this, &FilterDialog::onResetClicked);

    // connect - apply
    QPushButton *filterButton = new QPushButton("Apply");
    connect(filterButton, &QPushButton::clicked,
            this, &FilterDialog::onFilterClicked);

    buttonBox->addButton(filterButton, QDialogButtonBox::ButtonRole::ApplyRole);

    // connect - confirm
    QPushButton *confirmButton = new QPushButton("Confirm");
    connect(confirmButton, &QPushButton::clicked,
            this, &FilterDialog::accept);
    connect(confirmButton, &QPushButton::clicked,
            this, &FilterDialog::onFilterClicked);

    buttonBox->addButton(confirmButton, QDialogButtonBox::ButtonRole::AcceptRole);

    // connect - cancel
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked,
            this, &FilterDialog::reject);

    l->addWidget(buttonBox, 1, 0, 2, 0);
}

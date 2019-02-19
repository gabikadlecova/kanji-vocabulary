#include "addkanjiwidget.h"
#include "ui_addkanjiwidget.h"

#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

AddKanjiWidget::AddKanjiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddKanjiWidget)
{
    ui->setupUi(this);

    setupLayout();
}

AddKanjiWidget::~AddKanjiWidget()
{
    delete ui;
}


// if all fields are valid, signalizes that a new kanji compound should be added
void AddKanjiWidget::onAddClicked()
{
    auto k = kanjiField->text().toStdWString();
    auto r = readingField->text().toStdWString();
    auto m = meaningField->text().toStdWString();

    // all values must be non-empty
    if (k == L"" || r == L"" || m == L"") {

        // show message box and exit
        QMessageBox emptyBox;
        emptyBox.setText("Cannot add kanji");
        emptyBox.setInformativeText("Some of the fields are empty.");
        emptyBox.addButton(QMessageBox::Ok);
        emptyBox.exec();
        return;
    }

    // create a new compound without id or training data (will be set up in the lib)
    emit kanjiAddRequested(kcomp(k, r, m));
}


// on success, wipe all fields
void AddKanjiWidget::onAddSucceeded(kcomp kc)
{
    kanjiField->setText("");
    readingField->setText("");
    meaningField->setText("");
}


// set add form layout
void AddKanjiWidget::setupLayout()
{
    l = new QVBoxLayout();

    // kanji characters field
    QLabel *kanjiLabel = new QLabel("Kanji");
    kanjiField = new QLineEdit();

    auto font = kanjiField->font();
    font.setPixelSize(32);
    kanjiField->setFont(font);

    // reading field
    QLabel *readingLabel = new QLabel("Reading");
    readingField = new QLineEdit();

    // meaning field
    QLabel *meaningLabel = new QLabel("Meaning");
    meaningField = new QLineEdit();

    /* add widgets */
    l->addWidget(kanjiLabel);
    l->addWidget(kanjiField);

    l->addWidget(readingLabel);
    l->addWidget(readingField);

    l->addWidget(meaningLabel);
    l->addWidget(meaningField);

    // submit button
    setupButtons();

    l->addStretch();
    setLayout(l);
}


// adds the submit button
void AddKanjiWidget::setupButtons()
{
    QPushButton *addButton = new QPushButton("Add kanji");
    connect(addButton, &QPushButton::clicked,
            this, &AddKanjiWidget::onAddClicked);

    l->addWidget(addButton);
}

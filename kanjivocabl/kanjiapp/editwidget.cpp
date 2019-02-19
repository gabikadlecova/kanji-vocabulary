#include "editwidget.h"
#include "ui_editwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QMessageBox>


EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);

    setupLayout();
}

EditWidget::~EditWidget()
{
    delete ui;
}


// changes displayed compound data
void EditWidget::onKanjiChanged(kcomp &kc)
{
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));
}


// requests save of the compoind
void EditWidget::onSaveClicked()
{
    // field cannot be empty
    if (meaningField->text() == "" || readingField->text() == "") {

        QMessageBox emptyBox;
        emptyBox.setText("Cannot update kanji data");
        emptyBox.setInformativeText("Some of the fields are empty.");
        emptyBox.addButton(QMessageBox::Ok);
        emptyBox.exec();
        return;
    }

    curr_kanji->meaning = meaningField->text().toStdWString();
    curr_kanji->reading = readingField->text().toStdWString();

    emit kanjiUpdateSaved(*curr_kanji);
}


// sets up the layout
void EditWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupKanjiData();
    setupButtons();

    l->addStretch();
    setLayout(l);
}


// sets up the page fields
void EditWidget::setupKanjiData()
{
    QLabel *kanjiLabel = new QLabel();
    connect(this, &EditWidget::kanjiTextChanged,
            kanjiLabel, &QLabel::setText);

    auto font = kanjiLabel->font();
    font.setPixelSize(32);
    kanjiLabel->setFont(font);


    QLabel *readingLabel = new QLabel("Reading");
    readingField = new QLineEdit();
    connect(this, &EditWidget::kanjiReadingChanged,
            readingField, &QLineEdit::setText);

    QLabel *meaningLabel = new QLabel("Meaning");
    meaningField = new QLineEdit();
    connect(this, &EditWidget::kanjiMeaningChanged,
            meaningField, &QLineEdit::setText);


    l->addWidget(kanjiLabel);

    l->addWidget(readingLabel);
    l->addWidget(readingField);

    l->addWidget(meaningLabel);
    l->addWidget(meaningField);
}


// sets up the buttons
void EditWidget::setupButtons()
{
    // submit
    QPushButton *sb = new QPushButton("Save");
    connect(sb, &QPushButton::clicked,
            this, &EditWidget::onSaveClicked);

    l->addWidget(sb);
}

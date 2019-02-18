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

void EditWidget::onKanjiChanged(kcomp &kc)
{
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));
}

void EditWidget::onSaveClicked()
{
    if (meaning_l->text() == "" || reading_l->text() == "") {
        QMessageBox emptyBox;
        emptyBox.setText("Cannot update kanji data");
        emptyBox.setInformativeText("Some of the fields are empty.");
        emptyBox.addButton(QMessageBox::Ok);
        emptyBox.exec();
        return;
    }

    curr_kanji->meaning = meaning_l->text().toStdWString();
    curr_kanji->reading = reading_l->text().toStdWString();

    emit kanjiUpdateSaved(*curr_kanji);
}

void EditWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupKanjiData();
    setupButtons();

    l->addStretch();
    setLayout(l);
}


void EditWidget::setupKanjiData()
{
    QLabel *kanji_l = new QLabel();
    connect(this, &EditWidget::kanjiTextChanged, kanji_l, &QLabel::setText);

    auto font = kanji_l->font();
    font.setPixelSize(32);
    kanji_l->setFont(font);

    QLabel *readingLabel = new QLabel("Reading");
    reading_l = new QLineEdit();
    connect(this, &EditWidget::kanjiReadingChanged, reading_l, &QLineEdit::setText);

    QLabel *meaningLabel = new QLabel("Meaning");
    meaning_l = new QLineEdit();
    connect(this, &EditWidget::kanjiMeaningChanged, meaning_l, &QLineEdit::setText);


    l->addWidget(kanji_l);

    l->addWidget(readingLabel);
    l->addWidget(reading_l);

    l->addWidget(meaningLabel);
    l->addWidget(meaning_l);
}

void EditWidget::setupButtons()
{
    // submit
    QPushButton *sb = new QPushButton("Save");
    connect(sb, &QPushButton::clicked, this, &EditWidget::onSaveClicked);

    l->addWidget(sb);
}

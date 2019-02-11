#include "editwidget.h"
#include "ui_editwidget.h"

#include <QLabel>
#include <QPushButton>


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

void EditWidget::onSaveRequested()
{
    curr_kanji->meaning = meaning_l->text().toStdWString();
    curr_kanji->reading = reading_l->text().toStdWString();

    emit kanjiChanged(*curr_kanji);
}

void EditWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupKanjiData();
    setupButtons();

    setLayout(l);
}


void EditWidget::setupKanjiData()
{
    QLabel *kanji_l = new QLabel();
    connect(this, &EditWidget::kanjiTextChanged, kanji_l, &QLabel::setText);

    reading_l = new QLineEdit();
    connect(this, &EditWidget::kanjiReadingChanged, reading_l, &QLineEdit::setText);

    meaning_l = new QLineEdit();
    connect(this, &EditWidget::kanjiMeaningChanged, meaning_l, &QLineEdit::setText);

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);
}

void EditWidget::setupButtons()
{
    // submit
    QPushButton *sb = new QPushButton("Save");
    connect(sb, &QPushButton::clicked, this, &EditWidget::onSaveRequested);

    l->addWidget(sb);
}

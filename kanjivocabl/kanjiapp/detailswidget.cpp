#include <QLabel>
#include <QPushButton>

#include "detailswidget.h"
#include "ui_detailswidget.h"

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget(parent),
    curr_kanji(nullptr),
    ui(new Ui::DetailsWidget)
{
    ui->setupUi(this);

    editPageId = -1;
    setupLayout();
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}

void DetailsWidget::onKanjiChanged(const kcomp &kc)
{
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));
    emit kanjiLevelChanged(kc.get_level());
}

void DetailsWidget::onEditRequested()
{
    // todo connect to edit
    emit editPageOpened(editPageId);
}

void DetailsWidget::onKanjiDeleted()
{
    emit kanjiDeleted(curr_kanji->get_id());
}

void DetailsWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupKanjiData();
    setupButtons();

    setLayout(l);
}

void DetailsWidget::setupKanjiData()
{
    QLabel *kanji_l = new QLabel();
    connect(this, &DetailsWidget::kanjiTextChanged, kanji_l, &QLabel::setText);

    QLabel *reading_l = new QLabel();
    connect(this, &DetailsWidget::kanjiReadingChanged, reading_l, &QLabel::setText);

    QLabel *meaning_l = new QLabel();
    connect(this, &DetailsWidget::kanjiMeaningChanged, meaning_l, &QLabel::setText);

    // TODO level

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);
    // TODO add level
}

void DetailsWidget::setupButtons()
{
    // edit
    QPushButton *editButton = new QPushButton("Edit");
    l->addWidget(editButton);

    connect(editButton, &QPushButton::clicked, this, &DetailsWidget::onEditRequested);

    // delete
    QPushButton *deleteButton = new QPushButton("Delete");
    l->addWidget(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, &DetailsWidget::onKanjiDeleted);
}

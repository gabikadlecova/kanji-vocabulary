#include <QVBoxLayout>
#include <QLabel>

#include "detailswidget.h"
#include "ui_detailswidget.h"

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget(parent),
    curr_kanji(nullptr),
    ui(new Ui::DetailsWidget)
{
    ui->setupUi(this);

    setupLayout();
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}

void DetailsWidget::onKanjiChanged(kcomp &kc) {
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));
    emit kanjiLevelChanged(kc.get_level());
}

void DetailsWidget::setupLayout() {
    auto l = new QVBoxLayout();

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

    setLayout(l);
}

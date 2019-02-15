#include <QLabel>
#include <QPushButton>

#include <iomanip>
#include <ctime>
#include <sstream>

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

void DetailsWidget::onKanjiChanged(kcomp &kc)
{
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));
    emit kanjiLevelChanged(kc.get_level());

    auto time = std::chrono::system_clock::to_time_t(kc.get_last_rep());

    std::string time_str;
    std::stringstream ss;

    ss << std::put_time(std::localtime(&time), "%d/%m/%Y");

    emit kanjiLastRepChanged(QString::fromStdString(ss.str()));
}

void DetailsWidget::onEditClicked()
{
    // todo connect to edit
    emit editPageRequested(editPageId);
}

void DetailsWidget::onDeletionClicked()
{
    emit kanjiDeletionRequested(curr_kanji->get_id());
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

    QLabel *lastRep_l = new QLabel();
    connect(this, &DetailsWidget::kanjiLastRepChanged, lastRep_l, &QLabel::setText);

    // TODO level

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);
    l->addWidget(lastRep_l);
    // TODO add level

}

void DetailsWidget::setupButtons()
{
    // edit
    QPushButton *editButton = new QPushButton("Edit");
    connect(editButton, &QPushButton::clicked, this, &DetailsWidget::onEditClicked);

    // delete
    QPushButton *deleteButton = new QPushButton("Delete");
    connect(deleteButton, &QPushButton::clicked, this, &DetailsWidget::onDeletionClicked);

    // TODO reset button

    l->addWidget(editButton);
    l->addWidget(deleteButton);

}

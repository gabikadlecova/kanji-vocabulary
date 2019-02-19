#include "detailswidget.h"
#include "ui_detailswidget.h"

#include <QLabel>
#include <QPushButton>

#include <iomanip>
#include <ctime>
#include <sstream>


DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget(parent),
    curr_kanji(nullptr),
    ui(new Ui::DetailsWidget)
{
    ui->setupUi(this);

    editPageId = -1; // must be set externally
    setupLayout();
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}


// changes field of the detail page
void DetailsWidget::onKanjiChanged(kcomp &kc)
{
    curr_kanji = &kc;

    emit kanjiTextChanged(QString::fromWCharArray(kc.get_kanji().c_str()));
    emit kanjiReadingChanged(QString::fromWCharArray(kc.reading.c_str()));
    emit kanjiMeaningChanged(QString::fromWCharArray(kc.meaning.c_str()));

    // kc can be repeated after n days from the last repetition
    QString levelString = kc.get_level() == 1 ?
                "Repeated after 1 day" :
                QString("Repeated after %1 days").arg(kc.get_level());
    emit kanjiLevelChanged(levelString);

    // extract date from the timestamp
    auto time = std::chrono::system_clock::to_time_t(kc.get_last_rep());

    std::stringstream timeStr;
    timeStr << "Last repeated: " << std::put_time(std::localtime(&time), "%d/%m/%Y");

    emit kanjiLastRepChanged(QString::fromStdString(timeStr.str()));
}


// requests the edit page
void DetailsWidget::onEditClicked()
{
    emit editPageRequested(editPageId);
}


// requests kanji deletion by id
void DetailsWidget::onDeletionClicked()
{
    emit kanjiDeletionRequested(curr_kanji->get_id());
}


// sets up the layout
void DetailsWidget::setupLayout()
{
    l = new QVBoxLayout();

    setupKanjiData();
    setupButtons();

    l->addStretch();
    setLayout(l);
}


// sets up the detail
void DetailsWidget::setupKanjiData()
{
    QLabel *kanji_l = new QLabel();
    connect(this, &DetailsWidget::kanjiTextChanged,
            kanji_l, &QLabel::setText);

    auto font = kanji_l->font();
    font.setPixelSize(32);
    kanji_l->setFont(font);


    QLabel *reading_l = new QLabel();
    connect(this, &DetailsWidget::kanjiReadingChanged,
            reading_l, &QLabel::setText);

    QLabel *meaning_l = new QLabel();
    connect(this, &DetailsWidget::kanjiMeaningChanged,
            meaning_l, &QLabel::setText);

    QLabel *lastRep_l = new QLabel();
    connect(this, &DetailsWidget::kanjiLastRepChanged,
            lastRep_l, &QLabel::setText);

    QLabel *level_l = new QLabel();
    connect(this, &DetailsWidget::kanjiLevelChanged,
            level_l, &QLabel::setText);

    auto levelFont = level_l->font();
    levelFont.setItalic(true);
    level_l->setFont(levelFont);

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);
    l->addWidget(lastRep_l);
    l->addWidget(level_l);
}


// sets up the buttons
void DetailsWidget::setupButtons()
{
    // edit
    QPushButton *editButton = new QPushButton("Edit");
    connect(editButton, &QPushButton::clicked,
            this, &DetailsWidget::onEditClicked);

    // delete
    QPushButton *deleteButton = new QPushButton("Delete");
    connect(deleteButton, &QPushButton::clicked,
            this, &DetailsWidget::onDeletionClicked);

    // TODO reset button - resets kanji repetition time to "now" without changing level

    l->addWidget(editButton);
    l->addWidget(deleteButton);
}

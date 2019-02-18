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

    QString levelString = kc.get_level() == 1 ?
                "Repeated after 1 day" : QString("Repeated after %1 days").arg(kc.get_level());
    emit kanjiLevelChanged(levelString);

    auto time = std::chrono::system_clock::to_time_t(kc.get_last_rep());

    std::string time_str;
    std::stringstream ss;

    ss << "Last repeated: " << std::put_time(std::localtime(&time), "%d/%m/%Y");

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

    l->addStretch();
    setLayout(l);
}

void DetailsWidget::setupKanjiData()
{
    QLabel *kanji_l = new QLabel();
    connect(this, &DetailsWidget::kanjiTextChanged, kanji_l, &QLabel::setText);

    auto font = kanji_l->font();
    font.setPixelSize(32);
    kanji_l->setFont(font);


    QLabel *reading_l = new QLabel();
    connect(this, &DetailsWidget::kanjiReadingChanged, reading_l, &QLabel::setText);

    QLabel *meaning_l = new QLabel();
    connect(this, &DetailsWidget::kanjiMeaningChanged, meaning_l, &QLabel::setText);

    QLabel *lastRep_l = new QLabel();
    connect(this, &DetailsWidget::kanjiLastRepChanged, lastRep_l, &QLabel::setText);

    QLabel *level_l = new QLabel();
    connect(this, &DetailsWidget::kanjiLevelChanged, level_l, &QLabel::setText);

    auto levelFont = level_l->font();
    levelFont.setItalic(true);
    level_l->setFont(levelFont);

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);
    l->addWidget(lastRep_l);
    l->addWidget(level_l);
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

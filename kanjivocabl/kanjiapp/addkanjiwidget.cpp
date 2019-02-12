#include "addkanjiwidget.h"
#include "ui_addkanjiwidget.h"

#include <QPushButton>

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


void AddKanjiWidget::onAddClicked()
{
    auto k = kanji_l->text().toStdWString();
    auto r = reading_l->text().toStdWString();
    auto m = meaning_l->text().toStdWString();

    emit kanjiAddRequested(kcomp(k, r, m));
}

void AddKanjiWidget::setupLayout()
{
    l = new QVBoxLayout();

    kanji_l = new QLineEdit();
    reading_l = new QLineEdit();
    meaning_l = new QLineEdit();

    l->addWidget(kanji_l);
    l->addWidget(reading_l);
    l->addWidget(meaning_l);

    setupButtons();
    setLayout(l);
}

void AddKanjiWidget::setupButtons()
{
    QPushButton *addButton = new QPushButton("Add kanji");
    connect(addButton, &QPushButton::clicked,
            this, &AddKanjiWidget::onAddClicked);

    l->addWidget(addButton);
}

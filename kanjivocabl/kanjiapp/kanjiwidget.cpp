#include "kanjiwidget.h"
#include "ui_kanjiwidget.h"

#include <QPushButton>

KanjiWidget::KanjiWidget(QStackedWidget *st, QWidget *parent) : QWidget(parent),
    widgetCount(0), ui(new Ui::KanjiWidget)
{
    ui->setupUi(this);

    pageStack = st;
    setupLayout();
}

KanjiWidget::~KanjiWidget() {
    delete ui;
}

void KanjiWidget::setupLayout() {
    l = new QGridLayout();
    setLayout(l);
}

int KanjiWidget::addWidget(const QString &name, QWidget *widget, const QIcon *icon) {
    QPushButton *b = icon == nullptr ? new QPushButton(name) : new QPushButton(*icon, name);

    l->addWidget(b, widgetCount / 2, widgetCount % 2);
    widgetCount++;

    int id = pageStack->count();

    // signal a particular button being clicked
    connect(b, &QPushButton::clicked, this, [=](){
        emit pageButtonClicked(id);
    });

    pageStack->addWidget(widget);

    return id;
}


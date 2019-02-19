#include "kanjiwidget.h"
#include "ui_kanjiwidget.h"


KanjiWidget::KanjiWidget(QStackedWidget *st, QWidget *parent) : QWidget(parent),
    widgetCount(0), ui(new Ui::KanjiWidget)
{
    ui->setupUi(this);

    pageStack = st;
    setupLayout();
}

KanjiWidget::~KanjiWidget()
{
    delete ui;
}


// sets up the layout
void KanjiWidget::setupLayout()
{
    l = new QGridLayout();
    setLayout(l);
}


// adds a widget button
int KanjiWidget::addWidget(const QString &name, QWidget *widget, const QIcon *icon)
{
    QPushButton *b = icon == nullptr ?
                new QPushButton(name) : new QPushButton(*icon, name);

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


// adds a simple button (not associated with a widget or page id)
void KanjiWidget::addButton(QPushButton *button)
{
    l->addWidget(button, widgetCount / 2, widgetCount % 2);
    widgetCount++;
}


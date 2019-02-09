#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "kanjiwidget.h"
#include "kanjilistwidget.h"

#include "detailswidget.h"
#include "editwidget.h"

#include <QVBoxLayout>
#include <QPushButton>

MainWidget::MainWidget(QVector<kanji_data::kanji_compound> kanji,
                       QWidget *parent) :
    QWidget(parent),
    kanjiList(new KanjiListWidget(std::move(kanji))),
    idStack(new QStack<int>()),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setupLayout();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::pageChanged(int pageId) {
    pageStack->setCurrentIndex(pageId);

    idStack->push(pageId);

    emit page_opened();
}

void MainWidget::homeButtonClicked() {
    pageStack->setCurrentIndex(0);
    idStack->clear();
    idStack->push(0);

    emit home_opened();
}

void MainWidget::backButtonClicked() {
    const int currId = pageStack->currentIndex();
    if (currId == 0) {
        return;
    }

    idStack->pop();
    int nextId = idStack->top();

    if (nextId == 0) {
        homeButtonClicked();
        return;
    }
    pageStack->setCurrentIndex(nextId);

}

void MainWidget::setupLayout() {
    QVBoxLayout *l = new QVBoxLayout();

    /* menu : page = 1 : 10 */

    // menu setup
    setupMenu();
    QSizePolicy sizeUpper(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizeUpper.setVerticalStretch(1);
    menuSplitter->setSizePolicy(sizeUpper);

    l->addWidget(menuSplitter);

    // page setup
    setupPage();
    QSizePolicy sizeLower(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizeLower.setVerticalStretch(20);
    pageStack->setSizePolicy(sizeLower);

    l->addWidget(pageStack);

    // set as Widget layout
    setLayout(l);
}

void MainWidget::setupMenu() {
    menuSplitter = new QSplitter(Qt::Orientation::Horizontal);

    QPushButton *homeButton = new QPushButton("Home");
    menuSplitter->addWidget(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &MainWidget::homeButtonClicked);

    QPushButton *backButton = new QPushButton("Back");
    menuSplitter->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &MainWidget::backButtonClicked);
    connect(this, &MainWidget::page_opened, backButton, &QPushButton::show);
    connect(this, &MainWidget::home_opened, backButton, &QPushButton::hide);

    backButton->hide();

    QFrame *fr = new QFrame();
    menuSplitter->addWidget(fr);
}

void MainWidget::setupPage() {
    pageStack = new QStackedWidget();

    auto kv = new KanjiWidget(pageStack);
    pageStack->addWidget(kv);
    idStack->push(0);

    // tile page setup
    connect(kv, &KanjiWidget::pageButtonClicked, this, &MainWidget::pageChanged);

    // TODO could be defined elsewhere (separate function...)

    // kanji list setup
    connect(kanjiList, &KanjiListWidget::kanjiPageOpened, this, &MainWidget::pageChanged);
    kv->addWidget("Kanji list", kanjiList);

    // connect list to kanji detail
    auto dw = new DetailsWidget();
    kanjiList->kanjiPageId = pageStack->count();
    pageStack->addWidget(dw);

    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            dw, &DetailsWidget::onKanjiChanged);

    // TODO connect detail and edit
}

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "kanjiwidget.h"
#include "kanjilistwidget.h"

#include "detailswidget.h"
#include "editwidget.h"

#include <QVBoxLayout>
#include <QPushButton>

MainWidget::MainWidget(kanji_data::kanji_lib lib,
                       QWidget *parent) :
    QWidget(parent),
    lib(lib),
    idStack(new QStack<int>()),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    kanji_v = QVector<kanji_data::kanji_compound>::fromStdVector(lib.get_kanji());

    setupLayout();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::onPageChanged(int pageId) {
    pageStack->setCurrentIndex(pageId);

    idStack->push(pageId);

    emit pageOpened();
}

void MainWidget::onHomeButtonClicked() {
    pageStack->setCurrentIndex(0);
    idStack->clear();
    idStack->push(0);

    emit homeOpened();
}

void MainWidget::onBackButtonClicked() {
    const int currId = pageStack->currentIndex();
    if (currId == 0) {
        return;
    }

    idStack->pop();
    int nextId = idStack->top();

    if (nextId == 0) {
        onHomeButtonClicked();
        return;
    }
    pageStack->setCurrentIndex(nextId);

}

void MainWidget::onKanjiDeleted(kanji_data::kanji_compound::kanji_id id)
{
    // erase from lib only here
    lib.delete_kanji(id);

    // todo go back somewhere...
    onBackButtonClicked();
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
    connect(homeButton, &QPushButton::clicked, this, &MainWidget::onHomeButtonClicked);

    QPushButton *backButton = new QPushButton("Back");
    menuSplitter->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &MainWidget::onBackButtonClicked);
    connect(this, &MainWidget::pageOpened, backButton, &QPushButton::show);
    connect(this, &MainWidget::homeOpened, backButton, &QPushButton::hide);

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
    connect(kv, &KanjiWidget::pageButtonClicked, this, &MainWidget::onPageChanged);

    // TODO could be defined elsewhere (separate function...)

    // kanji list setup
    auto kanjiList = new KanjiListWidget(kanji_v);

    connect(kanjiList, &KanjiListWidget::kanjiPageOpened, this, &MainWidget::onPageChanged);
    kv->addWidget("Kanji list", kanjiList);

    // connect list to kanji detail
    int kanjiId = pageStack->count();

    auto dw = new DetailsWidget();
    kanjiList->kanjiPageId = kanjiId;
    pageStack->addWidget(dw);

    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            dw, &DetailsWidget::onKanjiChanged);

    // connect detail to edit
    auto ew = new EditWidget();
    dw->editPageId = pageStack->count();
    pageStack->addWidget(ew);

    connect(dw, &DetailsWidget::editPageOpened,
            this, &MainWidget::onPageChanged);

    // connect - kanji deletion
    connect(dw, &DetailsWidget::kanjiDeleted,
            this, &MainWidget::onKanjiDeleted);
    connect(dw, &DetailsWidget::kanjiDeleted,
            kanjiList, &KanjiListWidget::onKanjiDeleted);

    // TODO connect edit to detail to go back
}

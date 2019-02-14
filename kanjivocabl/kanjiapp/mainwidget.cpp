#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "kanjiwidget.h"
#include "kanjilistwidget.h"
#include "trainwidget.h"

#include "detailswidget.h"
#include "editwidget.h"
#include "addkanjiwidget.h"

#include <QPushButton>

MainWidget::MainWidget(kanji_data::kanji_lib lib,
                       QWidget *parent) :
    QWidget(parent),
    lib(lib),
    altMenuSplitter(nullptr),
    d(new FilterDialog(this)),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    kanji_v = QVector<kanji_data::kanji_compound>::fromStdVector(lib.get_kanji());

    connect(d, &FilterDialog::filterConfirmed,
            this, &MainWidget::onKanjiFiltered);

    setupLayout();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::onPageChanged(int pageId) {
    pageStack->setCurrentIndex(pageId);

    idStack.push(pageId);

    emit pageOpened();
}

void MainWidget::onHomeButtonClicked() {
    pageStack->setCurrentIndex(0);
    idStack.clear();
    idStack.push(0);

    emit homeOpened();
}

void MainWidget::onBackButtonClicked() {
    const int currId = pageStack->currentIndex();
    if (currId == 0) {
        return;
    }

    idStack.pop();
    int nextId = idStack.top();

    if (nextId == 0) {
        onHomeButtonClicked();
        return;
    }
    pageStack->setCurrentIndex(nextId);

}

void MainWidget::onCustomMenu(QSplitter *newMenu)
{
    l->removeWidget(menuSplitter);
    if (newMenu != nullptr) {
        l->insertWidget(0, newMenu);
        newMenu->show();
    }

    menuSplitter->hide();
    altMenuSplitter = newMenu;
}

void MainWidget::onDefaultMenu()
{
    if (altMenuSplitter != nullptr) {
        l->removeWidget(altMenuSplitter);
        altMenuSplitter->hide();
        altMenuSplitter = nullptr;
    }

    l->insertWidget(0, menuSplitter);
    menuSplitter->show();
}

void MainWidget::onKanjiDeleted(kcomp::kanji_id id)
{
    // erase from lib only here
    lib.delete_kanji(id);

    // TODO handle errs

    // todo go back somewhere...
    onBackButtonClicked();
}

void MainWidget::onKanjiChanged(kcomp kc)
{
    lib.update_kanji(std::move(kc));

    // TODO errors
    // TODO go back?
    onBackButtonClicked();
}

void MainWidget::onKanjiAdded(kcomp kc)
{
    auto new_kc = lib.add_kanji(std::move(kc.get_kanji()),
                                std::move(kc.reading),
                                std::move(kc.meaning));

    // TODO errors
    emit kanjiAdded(new_kc);
    onBackButtonClicked();
}

void MainWidget::onKanjiFiltered(FilterDialog::FilterMode fm, QString filterVal)
{
    std::vector<kcomp> filterRes;
    using Mode = FilterDialog::FilterMode;

    switch (fm) {
        case Mode::byKanji:
            filterRes = kanji_data::by_kanji(lib, filterVal.toStdWString());
            break;

        case Mode::byReading:
            filterRes = kanji_data::by_reading(lib, filterVal.toStdWString());
            break;

        case Mode::byMeaning:
            filterRes = kanji_data::by_meaning(lib, filterVal.toStdWString());
            break;

        case Mode::none:
            emit filterReset();
            return;

        default:
            // TODO some nasty error
            break;
    }

    emit kanjiFiltered(QVector<kcomp>::fromStdVector(std::move(filterRes)));
}

void MainWidget::onTrainingRequested()
{

}

void MainWidget::setupLayout() {
    l = new QVBoxLayout();

    // menu setup
    setupMenu();

    // page setup
    setupPage();

    // set as Widget layout
    setLayout(l);
}

void MainWidget::setupMenu() {
    QSizePolicy menuPolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    menuSplitter = new QSplitter(Qt::Orientation::Horizontal);

    // home button
    QPushButton *homeButton = new QPushButton("Home");
    homeButton->setSizePolicy(menuPolicy);

    menuSplitter->addWidget(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &MainWidget::onHomeButtonClicked);

    // back button
    QPushButton *backButton = new QPushButton("Back");
    backButton->setSizePolicy(menuPolicy);

    menuSplitter->addWidget(backButton);

    // menu connects
    connect(backButton, &QPushButton::clicked, this, &MainWidget::onBackButtonClicked);
    connect(this, &MainWidget::pageOpened, backButton, &QPushButton::show);
    connect(this, &MainWidget::homeOpened, backButton, &QPushButton::hide);

    backButton->hide();

    // button policy
    QFrame *fr = new QFrame();
    menuSplitter->addWidget(fr);
    menuSplitter->setSizePolicy(menuPolicy);

    l->addWidget(menuSplitter);
}

void MainWidget::setupPage() {
    pageStack = new QStackedWidget();

    auto kv = new KanjiWidget(pageStack);
    pageStack->addWidget(kv);
    idStack.push(0);

    // tile page setup
    connect(kv, &KanjiWidget::pageButtonClicked, this, &MainWidget::onPageChanged);

    // TODO could be defined elsewhere (separate function...)

    // kanji list setup
    auto kanjiList = new KanjiListWidget(kanji_v);

    connect(kanjiList, &KanjiListWidget::detailsPageRequested, this, &MainWidget::onPageChanged);
    kv->addWidget("Kanji list", kanjiList);

    // train MESSY, REWRITE (viz note)
    TrainWidget *tw = new TrainWidget();
    kv->addWidget("Train", tw);

    connect(tw, &TrainWidget::trainingEnded,
            this, &MainWidget::onHomeButtonClicked);

    connect(tw, &TrainWidget::customMenuShown,
            this, &MainWidget::onCustomMenu);
    connect(tw, &TrainWidget::customMenuHidden,
            this, &MainWidget::onDefaultMenu);


    connect(tw, &TrainWidget::trainingStarted,
            this, &MainWidget::onTrainingRequested);

    connect(this, &MainWidget::trainingDataChanged,
            tw, &TrainWidget::onTrainKanjiSet);

    connect(tw, &TrainWidget::trainingEnded,
            this, &MainWidget::onTrainingSubmitted);

    connect(tw, &TrainWidget::trainingDiscarded,
            this, &MainWidget::onTrainingFinished);

    // filter dialog
    connect(kanjiList, &KanjiListWidget::filterDialogRequested,
            d, &FilterDialog::show);

    // connect list to kanji detail
    int detailId = pageStack->count();

    auto dw = new DetailsWidget();
    kanjiList->detailPageId = detailId;
    pageStack->addWidget(dw);

    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            dw, &DetailsWidget::onKanjiChanged);

    // connect detail to edit
    auto ew = new EditWidget();
    dw->editPageId = pageStack->count();
    pageStack->addWidget(ew);


    connect(dw, &DetailsWidget::editPageRequested,
            this, &MainWidget::onPageChanged);

    // connect - kanji deletion
    connect(dw, &DetailsWidget::kanjiDeletionRequested,
            this, &MainWidget::onKanjiDeleted);
    connect(dw, &DetailsWidget::kanjiDeletionRequested,
            kanjiList, &KanjiListWidget::onKanjiDeleted);

    // edit page connects
    connect(ew, &EditWidget::kanjiUpdateSaved,
            dw, &DetailsWidget::onKanjiChanged);
    connect(ew, &EditWidget::kanjiUpdateSaved,
            this, &MainWidget::onKanjiChanged);

    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            ew, &EditWidget::onKanjiChanged);

    // add page
    auto aw = new AddKanjiWidget();
    kanjiList->addPageId = pageStack->count();
    pageStack->addWidget(aw);

    // connect add
    connect(aw, &AddKanjiWidget::kanjiAddRequested,
            this, &MainWidget::onKanjiAdded);
    connect(this, &MainWidget::kanjiAdded,
            kanjiList, &KanjiListWidget::onKanjiAdded);

    // connect to kanji list
    connect(kanjiList, &KanjiListWidget::addPageRequested,
            this, &MainWidget::onPageChanged);


    // MESSY
    // connect - filter
    connect(this, &MainWidget::kanjiFiltered,
            kanjiList, &KanjiListWidget::onKanjiFiltered);
    connect(this, &MainWidget::filterReset,
            kanjiList, &KanjiListWidget::onFilterReset);


    // pageStack setup
    QSizePolicy sizeLower(QSizePolicy::Preferred, QSizePolicy::Expanding);
    pageStack->setSizePolicy(sizeLower);

    l->addWidget(pageStack);
}

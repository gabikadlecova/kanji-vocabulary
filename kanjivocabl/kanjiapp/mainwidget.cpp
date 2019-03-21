#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "kanjiapp/kanjifilter.h"

#include "kanjiwidget.h"
#include "kanjilistwidget.h"
#include "trainwidget.h"
#include "detailswidget.h"
#include "editwidget.h"
#include "addkanjiwidget.h"

#include <QPushButton>
#include <QMessageBox>


MainWidget::MainWidget(std::string settingsName, QWidget *parent) :
    QWidget(parent),
    lm(new LibManip(settingsName, this)),
    lib(kanji_data::empty_lib()),
    d(new FilterDialog(this)),
    altMenuSplitter(nullptr),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // filter setup
    connect(d, &FilterDialog::filterConfirmed,
            this, &MainWidget::onKanjiFiltered);

    // save, load setup
    connect(this, &MainWidget::dataSaved,
            lm, &LibManip::onSaveData);

    connect(lm, &LibManip::noSaveFileSelected,
            this, &MainWidget::onSaveFailed);

    // save, load errors
    connect(lm, &LibManip::noLoadFileSelected,
            this, &MainWidget::onLoadFailed);

    connect(lm, &LibManip::dataLoaded,
            this, &MainWidget::onLibLoaded);

    connect(lm, &LibManip::loadFailed,
            this, &MainWidget::onBadFormat);

    // load the default library file
    if (lm->s.defaultPath != "") {
        lm->readLib(lm->s.defaultPath);
    }


    sd = new SettingsDialog(lm->s, this);
    connect(sd, &SettingsDialog::settingsSaveRequested,
            lm, &LibManip::onSaveSettings);
    connect(sd, &SettingsDialog::settingsLoadRequested,
            lm, &LibManip::onLoadSettings);

    connect(lm, &LibManip::settingsLoaded,
            sd, &SettingsDialog::onSettingsLoaded);

    setupLayout();
}

MainWidget::~MainWidget() {
    delete ui;
}


// is called when the current page changes
void MainWidget::onPageChanged(int pageId) {
    pageStack->setCurrentIndex(pageId);

    // enables "go back"
    idStack.push(pageId);

    emit pageOpened();
    emit pageNumberOpened(pageId);
}


// is called when the home page should be opened
void MainWidget::onHomeButtonClicked() {
    pageStack->setCurrentIndex(0);

    // clear history for "go back"
    idStack.clear();
    idStack.push(0);

    emit homeOpened();
}


// is called on "go back"
void MainWidget::onBackButtonClicked() {
    // home page is always the first in the stack
    const int currId = pageStack->currentIndex();
    if (currId == 0) {
        return;
    }

    // display the page opened before
    idStack.pop();
    int nextId = idStack.top();

    if (nextId == 0) {
        onHomeButtonClicked();
        return;
    }
    pageStack->setCurrentIndex(nextId);
}


// shows a custom menu instead of the default menu
void MainWidget::onCustomMenu(QSplitter *newMenu)
{
    // removes the currently shown menu
    if (altMenuSplitter != nullptr) {
        l->removeWidget(altMenuSplitter);
        altMenuSplitter->hide();
    }
    else {
        l->removeWidget(menuSplitter);
        menuSplitter->hide();
    }

    // shows the custom menu
    if (newMenu != nullptr) {
        l->insertWidget(0, newMenu);
        newMenu->show();
    }

    altMenuSplitter = newMenu;
}


// removes a custom menu and shows the default menu
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


// deletes a kanji compound from the library
void MainWidget::onKanjiDeleted(kcomp::kanji_id id)
{
    // erase from lib
    lib.delete_kanji(id);
    emit dataSaved(lib, true);

    // assumes that delete is called from a detail page - should be changed otherwise
    onBackButtonClicked();
}


// updates a kanji compound in the library
void MainWidget::onKanjiChanged(kcomp kc)
{
    // update in the lib
    lib.update_kanji(std::move(kc));
    emit dataSaved(lib, true);

    // assumes that update is called from a detail page - should be changed otherwise
    onBackButtonClicked();
}


// adds a kanji compound to the library
void MainWidget::onKanjiAdded(kcomp kc)
{
    // generally user should not add a kanji which is already present in the library
    try {
        auto new_kc = lib.add_kanji(std::move(kc.get_kanji()),
                                    std::move(kc.reading),
                                    std::move(kc.meaning));
        emit dataSaved(lib, true);

        emit kanjiAdded(new_kc);
        onBackButtonClicked();
    }
    catch (const kanji_data::duplicate_error &e) {
        QMessageBox addFailBox;
        addFailBox.setText("Add failed");
        addFailBox.setInformativeText("This word has already been added to the library.");
        addFailBox.setStandardButtons(QMessageBox::Ok);
        addFailBox.exec();
    }
}


// applies a filter to the library data
void MainWidget::onKanjiFiltered(FilterDialog::FilterMode fm, QString filterVal)
{  
    using Mode = FilterDialog::FilterMode;

    QVector<kcomp> filterRes;

    // empty string means no data
    if (fm != Mode::none && filterVal == "") {
        emit kanjiFiltered(QVector<kcomp>());
        return;
    }

    // filter the library
    switch (fm) {
        case Mode::none:
            emit filterReset();
            return;

        case Mode::byKanji:
        case Mode::byReading:
        case Mode::byMeaning:
            filterRes = get_filtered(lib, fm, filterVal);
            break;

        // if there were a new enum value added, error
        default:
            throw std::logic_error("Invalid filter mode.");
    }

    emit kanjiFiltered(std::move(filterRes));
}


// fetches available training data
void MainWidget::onTrainingRequested()
{
    auto trainKanji = get_due_today(lib, lm->s.kanjiPerRep);

    emit trainingDataChanged(std::move(trainKanji));
}


// updates kanji compounds with training results
void MainWidget::onTrainingSubmitted(const QVector<kcomp> &trainedKanji)
{
    std::for_each(trainedKanji.begin(), trainedKanji.end(), [=](const kcomp &kc){
        lib.update_kanji(kc);
    });
    emit dataSaved(lib, true);
    emit kanjiLoaded(QVector<kcomp>::fromStdVector(this->lib.get_kanji()));

    onTrainingFinished();
}


// finishes a training, does not update data
void MainWidget::onTrainingFinished()
{
    onHomeButtonClicked();
}


// loads new library data
void MainWidget::onLibLoaded(kanji_data::kanji_lib lib)
{
    this->lib = std::move(lib);
    emit kanjiLoaded(QVector<kcomp>::fromStdVector(this->lib.get_kanji()));
}


// tried to load data, format was bad
void MainWidget::onBadFormat()
{
    QMessageBox loadFailBox;
    loadFailBox.setText("Load failed");
    loadFailBox.setInformativeText("Input file format is not valid.");
    loadFailBox.setStandardButtons(QMessageBox::Ok);

    loadFailBox.exec();
}


// user aborted the load dialog
void MainWidget::onLoadFailed()
{
    QMessageBox loadFailBox;
    loadFailBox.setText("Load failed");
    loadFailBox.setInformativeText("No file was selected.");
    loadFailBox.setStandardButtons(QMessageBox::Ok);

    loadFailBox.exec();
}


// user aborted the save dialog
void MainWidget::onSaveFailed()
{
    QMessageBox saveFailBox;
    saveFailBox.setText("Save failed");
    saveFailBox.setInformativeText("No file was selected.");
    saveFailBox.setStandardButtons(QMessageBox::Ok);

    saveFailBox.exec();
}


// sets up the layout
void MainWidget::setupLayout() {
    setWindowTitle("Kanji vocabulary");
    l = new QVBoxLayout();

    // menu setup
    setupMenu();

    // title setup
    title = new QLabel("漢字");

    auto titleFont = title->font();
    titleFont.setPixelSize(32);
    title->setFont(titleFont);

    title->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    title->setContentsMargins(0,20,0,0);

    l->addWidget(title);

    // main widget connects
    connect(this, &MainWidget::pageNumberOpened,
            this, [=](int no){
        if (no == 0) {
            title->show();
        }
        else {
            title->hide();
        }
    });

    connect(this, &MainWidget::homeOpened,
            title, &QLabel::show);

    // page setup
    setupPage();

    // set as Widget layout
    setLayout(l);
}


// sets up the default menu
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


// sets up pages, connects pages together
void MainWidget::setupPage() {
    pageStack = new QStackedWidget();

    /* KanjiWidget - the menu page */
    auto kv = new KanjiWidget(pageStack);
    pageStack->addWidget(kv);
    idStack.push(0);

    // tile menu setup
    connect(kv, &KanjiWidget::pageButtonClicked,
            this, &MainWidget::onPageChanged);


    /* KanjiListWidget - list of kanji compounds*/
    auto kanjiList = new KanjiListWidget(QVector<kcomp>::fromStdVector(lib.get_kanji()));

    // update with loaded data
    connect(this, &MainWidget::kanjiLoaded,
            kanjiList, &KanjiListWidget::onKanjiLoaded);

    // open detail page of the selected compound
    connect(kanjiList, &KanjiListWidget::detailPageRequested,
            this, &MainWidget::onPageChanged);

    kv->addWidget("Kanji list", kanjiList);


    /* TrainWidget - flashcards */
    TrainWidget *tw = new TrainWidget(lm->s);

    // save trainId - pageStack id
    int trainId = pageStack->count();
    kv->addWidget("Train", tw);

    // start training
    connect(this, &MainWidget::pageNumberOpened,
            this, [=](int pageId){
        if (pageId == trainId) {
            this->onTrainingRequested();
        }
    });
    connect(this, &MainWidget::trainingDataChanged,
            tw, &TrainWidget::onTrainKanjiSet);

    // end training, save changes
    connect(tw, &TrainWidget::trainingEnded,
            this, &MainWidget::onHomeButtonClicked);
    connect(tw, &TrainWidget::trainingEnded,
            this, &MainWidget::onTrainingSubmitted);

    // end training, do not save changes
    connect(tw, &TrainWidget::trainingDiscarded,
            this, &MainWidget::onTrainingFinished);


    // pass settings to train widget
    connect(lm, &LibManip::settingsLoaded,
            tw, &TrainWidget::onSettingsLoaded);


    // custom menu
    connect(tw, &TrainWidget::customMenuShown,
            this, &MainWidget::onCustomMenu);
    connect(tw, &TrainWidget::customMenuHidden,
            this, &MainWidget::onDefaultMenu);


    /* Load button - load a lib */
    QPushButton *loadButton = new QPushButton("Load data");
    connect(loadButton, &QPushButton::clicked,
            lm, &LibManip::onLoadData);

    kv->addButton(loadButton);

    /* Settings dialog */
    QPushButton *settingsButton = new QPushButton("Settings");
    connect(settingsButton, &QPushButton::clicked,
            sd, &SettingsDialog::show);

    kv->addButton(settingsButton);

    /* FilterDialog - open from kanjiList */
    connect(kanjiList, &KanjiListWidget::filterDialogRequested,
            d, &FilterDialog::show);

    connect(this, &MainWidget::kanjiFiltered,
            kanjiList, &KanjiListWidget::onKanjiFiltered);
    connect(this, &MainWidget::filterReset,
            kanjiList, &KanjiListWidget::onFilterReset);

    /* DetailWidget - kanji compound detail page */

    auto dw = new DetailsWidget();

    // save detailId - pageStack id
    int detailId = pageStack->count();
    kanjiList->detailPageId = detailId;
    pageStack->addWidget(dw);

    // selected kanji shown in detail
    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            dw, &DetailsWidget::onKanjiChanged);

    /* EditWidget - kanji compound edit page */
    auto ew = new EditWidget();

    // edit is opened from detail
    dw->editPageId = pageStack->count();
    pageStack->addWidget(ew);

    connect(dw, &DetailsWidget::editPageRequested,
            this, &MainWidget::onPageChanged);

    // kanji deletion
    connect(dw, &DetailsWidget::kanjiDeletionRequested,
            this, &MainWidget::onKanjiDeleted);
    connect(dw, &DetailsWidget::kanjiDeletionRequested,
            kanjiList, &KanjiListWidget::onKanjiDeleted);

    // kanji update
    connect(ew, &EditWidget::kanjiUpdateSaved,
            dw, &DetailsWidget::onKanjiChanged);
    connect(ew, &EditWidget::kanjiUpdateSaved,
            this, &MainWidget::onKanjiChanged);

    // selected kanji shown in edit
    connect(kanjiList, &KanjiListWidget::currentKanjiChanged,
            ew, &EditWidget::onKanjiChanged);


    /* AddWidget - add a new kanji compound */
    auto aw = new AddKanjiWidget();

    // save addId - pageStack id
    kanjiList->addPageId = pageStack->count();
    pageStack->addWidget(aw);

    // add to library and list
    connect(aw, &AddKanjiWidget::kanjiAddRequested,
            this, &MainWidget::onKanjiAdded);
    connect(this, &MainWidget::kanjiAdded,
            kanjiList, &KanjiListWidget::onKanjiAdded);
    connect(this, &MainWidget::kanjiAdded,
            aw, &AddKanjiWidget::onAddSucceeded);

    // opened from the list
    connect(kanjiList, &KanjiListWidget::addPageRequested,
            this, &MainWidget::onPageChanged);


    // pageStack layout setup
    QSizePolicy stackSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    pageStack->setSizePolicy(stackSizePolicy);

    l->addWidget(pageStack);
}

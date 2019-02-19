#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

#include <QPushButton>


KanjiListWidget::KanjiListWidget(QVector<kcomp> kanji,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiListWidget)
{
    ui->setupUi(this);


    // setup model

    // initial model
    model = new KanjiListModel(std::move(kanji));
    // no filter
    filterModel = nullptr;

    listView = new QListView();
    listView->setModel(model);


    // kanji compound selection
    connect(listView, &QListView::clicked,
            this, &KanjiListWidget::onKanjiClicked);

    // must be set externally
    detailPageId = -1;
    addPageId = -1;

    setupLayout();
}

KanjiListWidget::~KanjiListWidget()
{
    delete ui;
}


// sets up the layout
void KanjiListWidget::setupLayout()
{
    l = new QGridLayout();
    setupHeader();

    l->addWidget(listView, 1, 0, 1, 2);
    setLayout(l);
}


// sets up the header buttons
void KanjiListWidget::setupHeader()
{
    QPushButton *addButton = new QPushButton("Add kanji");
    connect(addButton, &QPushButton::clicked,
            this, &KanjiListWidget::onAddKanjiClicked);

    QPushButton *filterButton = new QPushButton("Filter");
    connect(filterButton, &QPushButton::clicked,
            this, &KanjiListWidget::onFilterClicked);

    l->addWidget(addButton, 0, 0);
    l->addWidget(filterButton, 0, 1);
}


// new model loaded
void KanjiListWidget::onKanjiLoaded(QVector<kcomp> kanji)
{
    delete model;
    model = new KanjiListModel(std::move(kanji));

    listView->setModel(model);
}


// kanji compound deleted from the list
void KanjiListWidget::onKanjiDeleted(kcomp::kanji_id id)
{
    int row = model->getKanjiRow(id);

    model->removeRows(row, 1);

    // should be removed both from filtered and unfiltered model
    if (filterModel != nullptr) {
        row = filterModel->getKanjiRow(id);

        // the kanji compound is not a part of the filtered data
        if (row == -1) {
            return;
        }

        filterModel->removeRows(row, 1);
    }
}


// kanji compound added to the list (not to the filtered data)
void KanjiListWidget::onKanjiAdded(kcomp kc)
{
    int row = model->rowCount();

    model->insertRows(row, 1);
    model->setKanji(row, std::move(kc));

    onFilterReset();
}


// filter applied
void KanjiListWidget::onKanjiFiltered(QVector<kcomp> filter)
{
    delete filterModel;
    filterModel = new KanjiListModel(std::move(filter));
    listView->setModel(filterModel);
}


// unfiltered data displayed
void KanjiListWidget::onFilterReset()
{
    listView->setModel(model);
    delete filterModel;
    filterModel = nullptr;
}


// kanji compound selected
void KanjiListWidget::onKanjiClicked(const QModelIndex &index)
{
    auto &&kc = filterModel == nullptr ? model->getKanji(index) :
                                         filterModel->getKanji(index);

    emit currentKanjiChanged(kc);
    emit detailPageRequested(detailPageId);
}


// add kanji page
void KanjiListWidget::onAddKanjiClicked()
{
    emit addPageRequested(addPageId);
}


// filter dialog
void KanjiListWidget::onFilterClicked()
{
    emit filterDialogRequested();
}


KanjiListModel::KanjiListModel(QVector<kcomp> kanji, QObject *parent) :
    QAbstractListModel(parent), kanji(std::move(kanji)) {}


// returns number of kanji compounds
int KanjiListModel::rowCount(const QModelIndex &parent) const
{
    return kanji.size();
}


// returns string representation of the kanji compound
QVariant KanjiListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= kanji.size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QString("%1 - %2").arg(
                    QString::fromStdWString(kanji[index.row()].get_kanji()),
                    QString::fromStdWString(kanji[index.row()].meaning)
                );
    }
    else {
        return QVariant();
    }
}


// gets the kanji on the given row
KanjiListModel::kcomp &KanjiListModel::getKanji(const QModelIndex &ind)
{
    return kanji[ind.row()];
}


// finds the row index of the compound with id
int KanjiListModel::getKanjiRow(kcomp::kanji_id id)
{
    auto it = std::find_if(kanji.begin(), kanji.end(), [=](kcomp kc) {
        return kc.get_id() == id;
    });

    if (it == kanji.end()) {
        return -1;
    }

    // QVector size limitation
    int d =  std::distance(kanji.begin(), it);
    return d;
}


// remove rows from the model
bool KanjiListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    kanji.erase(kanji.begin() + row, kanji.begin() + row + count);

    endRemoveRows();
    return true;
}


// add rows to the model
bool KanjiListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    kanji.insert(row, count, kcomp());

    endInsertRows();
    return true;
}


// set data of the row
void KanjiListModel::setKanji(int row, kcomp kc)
{
    kanji[row] = std::move(kc);
}

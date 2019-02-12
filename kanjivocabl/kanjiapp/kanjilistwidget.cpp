#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

#include <QPushButton>


KanjiListWidget::KanjiListWidget(QVector<kanji_data::kanji_compound> kanji,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiListWidget)
{
    ui->setupUi(this);

    model = new KanjiListModel(std::move(kanji));
    filterModel = nullptr;

    listView = new QListView();
    listView->setModel(model);

    // connect handler to QListView signal
    connect(listView, &QListView::clicked, this, &KanjiListWidget::onKanjiClicked);

    detailPageId = -1;

    setupLayout();
}

KanjiListWidget::~KanjiListWidget()
{
    delete ui;
}

void KanjiListWidget::setupLayout()
{
    l = new QGridLayout();
    setupHeader();

    l->addWidget(listView, 1, 0, 1, 2);
    setLayout(l);
}

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

void KanjiListWidget::onKanjiDeleted(kanji_data::kanji_compound::kanji_id id)
{
    int row = model->getKanjiRow(id);

    // TODO check retval
    model->removeRows(row, 1);

    if (filterModel != nullptr) {
        row = filterModel->getKanjiRow(id);
        if (row == -1) {
            return;
        }

        // TODO check retval
        filterModel->removeRows(row, 1);
    }
}

void KanjiListWidget::onKanjiAdded(kanji_data::kanji_compound kc)
{
    int row = model->rowCount();

    // TODO check retval;
    model->insertRows(row, 1);
    model->setKanji(row, std::move(kc));

    // maybe not slot directly...
    onFilterReset();
}

void KanjiListWidget::onKanjiFiltered(QVector<kanji_data::kanji_compound> filter)
{
    filterModel = new KanjiListModel(std::move(filter));
    listView->setModel(filterModel);
}

void KanjiListWidget::onFilterReset()
{
    listView->setModel(model);
    delete filterModel;
    filterModel = nullptr;
}

void KanjiListWidget::onKanjiClicked(const QModelIndex &index)
{
    auto &&kc = filterModel == nullptr ? model->getKanji(index) :
                                         filterModel->getKanji(index);

    // change selected kanji, change stacked page id
    emit currentKanjiChanged(kc);
    emit detailsPageRequested(detailPageId);
}

void KanjiListWidget::onAddKanjiClicked()
{
    emit addPageRequested(addPageId);
}

void KanjiListWidget::onFilterClicked()
{
    emit filterDialogRequested();
}

KanjiListModel::KanjiListModel(QVector<kcomp> kanji, QObject *parent) :
    QAbstractListModel(parent), kanji(std::move(kanji)) {}


int KanjiListModel::rowCount(const QModelIndex &parent) const
{
    return kanji.size();
}

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

KanjiListModel::kcomp &KanjiListModel::getKanji(const QModelIndex &ind)
{
    return kanji[ind.row()];
}

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

bool KanjiListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    // TODO retval?
    beginRemoveRows(parent, row, row + count - 1);
    kanji.erase(kanji.begin() + row, kanji.begin() + row + count);

    endRemoveRows();
    return true;
}

bool KanjiListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    // TODO retval?
    beginInsertRows(parent, row, row + count - 1);
    kanji.insert(row, count, kcomp());

    endInsertRows();
    return true;
}

void KanjiListModel::setKanji(int row, kcomp kc)
{
    kanji[row] = std::move(kc);
}
//todo listview, connect signals as to actualize list

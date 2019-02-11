#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

#include <QGridLayout>


KanjiListWidget::KanjiListWidget(QVector<kanji_data::kanji_compound> &kanji,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiListWidget)
{
    ui->setupUi(this);

    model = new KanjiListModel(kanji);
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
    QGridLayout *l = new QGridLayout();
    l->addWidget(listView);
    setLayout(l);
}

void KanjiListWidget::onKanjiDeleted(kanji_data::kanji_compound::kanji_id id)
{
    int row = model->getKanjiRow(id);

    model->removeRows(row, 1);
}

void KanjiListWidget::onKanjiClicked(const QModelIndex &index)
{
    auto &&kc = model->getKanji(index);

    // change selected kanji, change stacked page id
    emit currentKanjiChanged(kc);
    emit kanjiPageOpened(detailPageId);
}

KanjiListModel::KanjiListModel(QVector<kcomp> &kanji, QObject *parent) :
    QAbstractListModel(parent), kanji(kanji) {}


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
        return QString::fromStdWString(kanji[index.row()].get_kanji());
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

    // QVector size limitation
    int d =  std::distance(kanji.begin(), it);
    return d;
}

bool KanjiListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    kanji.erase(kanji.begin() + row, kanji.begin() + row + count);

    endRemoveRows();
    return true;
}
//todo listview, connect signals as to actualize list

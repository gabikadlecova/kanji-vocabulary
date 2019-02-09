#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

#include <QGridLayout>


KanjiListWidget::KanjiListWidget(QVector<kanji_data::kanji_compound> kanji,
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

    kanjiPageId = -1;

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

void KanjiListWidget::onKanjiClicked(const QModelIndex &index)
{
    auto kc = model->get_kanji(index);

    // change selected kanji, change stacked page id
    emit currentKanjiChanged(kc);
    emit kanjiPageOpened(kanjiPageId);
}

KanjiListModel::KanjiListModel(QVector<kanji_compound> kanji, QObject *parent) :
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
        return QString::fromStdWString(kanji[index.row()].get_kanji());
    }
    else {
        return QVariant();
    }
}

kanji_data::kanji_compound &KanjiListModel::get_kanji(const QModelIndex &ind) {
    // TODO check
    return kanji[ind.row()];
}

//todo listview, connect signals as to actualize list

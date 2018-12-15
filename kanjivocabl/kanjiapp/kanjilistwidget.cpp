#include "kanjilistwidget.h"
#include "ui_kanjilistwidget.h"

#include <limits>
#include <stdexcept>

KanjiListWidget::KanjiListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanjiListWidget)
{
    ui->setupUi(this);
}

KanjiListWidget::~KanjiListWidget()
{
    delete ui;
}

KanjiListModel::KanjiListModel(QVector<kanji_compound> *kanji, QObject *parent) :
    QAbstractListModel(parent), kanji(kanji){}

int KanjiListModel::rowCount(const QModelIndex &parent) const
{
    return kanji->size();
}

QVariant KanjiListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= kanji->size()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QString::fromStdWString((*kanji)[index.row()].get_kanji());
    }
    else {
        return QVariant();
    }
}

//todo listview, connect signals as to actualize list

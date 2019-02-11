#ifndef KANJILISTWIDGET_H
#define KANJILISTWIDGET_H

#include <QWidget>
#include <QAbstractListModel>
#include <QListView>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class KanjiListWidget;
}

class KanjiListModel;

class KanjiListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanjiListWidget(QVector<kanji_data::kanji_compound> &kanji,
                             QWidget *parent = nullptr);
    ~KanjiListWidget();
    int detailPageId;

signals:
    void currentKanjiChanged(kanji_data::kanji_compound &kc);
    void kanjiPageOpened(int pageId);

public slots:
    void onKanjiDeleted(kanji_data::kanji_compound::kanji_id id);

private slots:
    void onKanjiClicked(const QModelIndex &index);

private:
    void setupLayout();

    KanjiListModel *model;
    QListView *listView;

    Ui::KanjiListWidget *ui;
};

class KanjiListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    using kcomp = kanji_data::kanji_compound;

    explicit KanjiListModel(QVector<kcomp> &kanji, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    kcomp &getKanji(const QModelIndex &ind);

    int getKanjiRow(kcomp::kanji_id id);
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;
//signals:
//    void kanjiChanged();

//private slots:
//    void kanjiDataChanged();

private:
    QVector<kcomp> &kanji;
};

#endif // KANJILISTWIDGET_H

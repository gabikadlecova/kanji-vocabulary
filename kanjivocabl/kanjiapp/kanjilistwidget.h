#ifndef KANJILISTWIDGET_H
#define KANJILISTWIDGET_H

#include <QWidget>
#include <QAbstractListModel>
#include <QListView>
#include <QGridLayout>

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
    int addPageId;

signals:
    void currentKanjiChanged(kanji_data::kanji_compound &kc);
    void detailsPageRequested(int pageId);
    void addPageRequested(int pageId);
    void filterDialogRequested();

public slots:
    void onKanjiDeleted(kanji_data::kanji_compound::kanji_id id);
    void onKanjiAdded(kanji_data::kanji_compound kc);

private slots:
    void onKanjiClicked(const QModelIndex &index);
    void onAddKanjiClicked();
    void onFilterClicked();

private:
    void setupLayout();
    void setupHeader();

    QGridLayout *l;

    KanjiListModel *model;
    // TODO slots for filter/normal mode change, delete etc in both...
    KanjiListModel *filterModel;

    QListView *listView;

    Ui::KanjiListWidget *ui;
};


class KanjiListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    using kcomp = kanji_data::kanji_compound;

    explicit KanjiListModel(QVector<kcomp> &kanji, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    kcomp &getKanji(const QModelIndex &ind);

    int getKanjiRow(kcomp::kanji_id id);
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;

    virtual bool insertRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;

    void setKanji(int row, kcomp kc);
//signals:
//    void kanjiChanged();

//private slots:
//    void kanjiDataChanged();

private:
    QVector<kcomp> &kanji;
};

#endif // KANJILISTWIDGET_H

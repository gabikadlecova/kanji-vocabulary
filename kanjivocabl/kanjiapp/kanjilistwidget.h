#ifndef KANJILISTWIDGET_H
#define KANJILISTWIDGET_H

#include "kanjiapp/KanjiData.h"

#include <QWidget>
#include <QAbstractListModel>
#include <QListView>
#include <QGridLayout>

namespace Ui {
class KanjiListWidget;
}

class KanjiListModel;

/*!
 * \brief The KanjiListWidget class represents a list of kanji compounds.
 */
class KanjiListWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

public:
    /*!
     * \brief KanjiListWidget Initializes a kanji list page.
     * \param kanji Initial kanji compound list
     * \param parent Qt parent
     */
    explicit KanjiListWidget(QVector<kcomp> kanji,
                             QWidget *parent = nullptr);
    ~KanjiListWidget();

    int detailPageId; /*!< Id link of the detail page for detailPageRequested() signal */
    int addPageId; /*!< Id link of the add page for addPageRequested() signal */

signals:
    /*!
     * \brief currentKanjiChanged Signalizes that the selected kanji compound changed.
     * \param kc Current kanji compound.
     */
    void currentKanjiChanged(kcomp &kc);
    /*!
     * \brief detailPageRequested Requests the detail page.
     * \param pageId Externally set detail page id.
     */
    void detailPageRequested(int pageId);
    /*!
     * \brief addPageRequested Requests the add page.
     * \param pageId Externally set add page id.
     */
    void addPageRequested(int pageId);
    /*!
     * \brief filterDialogRequested Requests a dialog for list filtering.
     */
    void filterDialogRequested();

public slots:
    /*!
     * \brief onKanjiLoaded Replaces the old list with a new kanji compound list.
     * \param kanji New kanji compounds to be displayed.
     */
    void onKanjiLoaded(QVector<kcomp> kanji);

    /*!
     * \brief onKanjiDeleted Deletes a kanji compound with the given id from the list.
     * \param id Identifier of the kanji compound which should be deleted.
     */
    void onKanjiDeleted(kcomp::kanji_id id);
    /*!
     * \brief onKanjiAdded Adds a new kanji compound to the list.
     * \param kc Compound to be added to the list.
     */
    void onKanjiAdded(kcomp kc);
    /*!
     * \brief onKanjiFiltered Applies a filter to the list, changes the list model
     * \param filter List of compounds filtered
     */
    void onKanjiFiltered(QVector<kcomp> filter);
    /*!
     * \brief onFilterReset Resets the filter, changes list model to unfiltered data
     */
    void onFilterReset();

private slots:
    /*!
     * \brief onKanjiClicked Is called when the user selects a kanji compound.
     * \param index Index of the compound in the list.
     */
    void onKanjiClicked(const QModelIndex &index);
    /*!
     * \brief onAddKanjiClicked Is called when the user wants to open the add page.
     */
    void onAddKanjiClicked();
    /*!
     * \brief onFilterClicked Is called when the user wants to filter the data.
     */
    void onFilterClicked();

private:
    /*!
     * \brief setupLayout Sets up the layout.
     */
    void setupLayout();
    /*!
     * \brief setupHeader Sets up the buttons above the list.
     */
    void setupHeader();

    QGridLayout *l;

    KanjiListModel *model;
    KanjiListModel *filterModel;

    QListView *listView;

    Ui::KanjiListWidget *ui;
};


/*!
 * \brief The KanjiListModel class represents a model containing compound data.
 */
class KanjiListModel : public QAbstractListModel
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;
public:

    /*!
     * \brief KanjiListModel Constructs the list model with compound data.
     * \param kanji Model data
     * \param parent Qt parent
     */
    explicit KanjiListModel(QVector<kcomp> kanji,
                            QObject *parent = nullptr);
    virtual ~KanjiListModel() override = default;

    /*!
     * \brief rowCount Returns the number of compounds in the model.
     * \param parent
     * \return Number of compounds.
     */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /*!
     * \brief data Gets data according to the specified index.
     * \param index Row index.
     * \param role
     * \return Kanji compound string on the specified index.
     */
    virtual QVariant data(const QModelIndex &index, int role) const override;

    /*!
     * \brief getKanji Gets a kanji compound according to the specified index.
     * \param ind Row index.
     * \return Kanji compound according to the index.
     */
    kcomp &getKanji(const QModelIndex &ind);

    /*!
     * \brief getKanjiRow Gets row index of the kanji with \p id identifier.
     * \param id Identifier of the compound.
     * \return Row index of the compound.
     */
    int getKanjiRow(kcomp::kanji_id id);
    /*!
     * \brief removeRows Removes rows from the model.
     * \param row Row from which the removal occurs.
     * \param count Number of rows to be removed.
     * \param parent
     * \return True on success.
     */
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;
    /*!
     * \brief insertRows Inserts empty rows to the model.
     * \param row Row where the insertion occurs.
     * \param count Number of rows to be inserted.
     * \param parent
     * \return True on success.
     */
    virtual bool insertRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) override;

    /*!
     * \brief setKanji Sets a row value to the specified compound.
     * \param row Row which should be set.
     * \param kc Kanji compound data.
     */
    void setKanji(int row, kcomp kc);

private:
    QVector<kcomp> kanji;
};

#endif // KANJILISTWIDGET_H

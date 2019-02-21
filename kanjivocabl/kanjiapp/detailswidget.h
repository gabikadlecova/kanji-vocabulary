#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include "kanjiapp/KanjiData.h"

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class DetailsWidget;
}

/*!
 * \brief The DetailsWidget class represents a detail page of a kanji compound.
 */
class DetailsWidget : public QWidget
{
    Q_OBJECT
    using kcomp = kanji_data::kanji_compound;

public:
    explicit DetailsWidget(QWidget *parent = nullptr);
    virtual ~DetailsWidget() override;

    int editPageId; /*!< Id link of the edit page for editPageRequested() signal */

signals:
    /*!
     * \brief kanjiTextChanged signalizes when compound kanji characters change
     * \param text updated kanji characters
     * Is emitted when the current kanji compound of the detail page changes
     */
    void kanjiTextChanged(const QString &text);
    /*!
     * \brief kanjiReadingChanged signalizes when the compound reading changes
     * \param text updated kanji reading
     * Is emitted when the current kanji compound of the detail page changes
     */
    void kanjiReadingChanged(const QString &text);
    /*!
     * \brief kanjiMeaningChanged signalizes when the compound meaning changes
     * \param text updated kanji meaning
     * Is emitted when the current kanji compound of the detail page changes
     */
    void kanjiMeaningChanged(const QString &text);
    /*!
     * \brief kanjiLevelChanged signalizes when the compound level changes
     * \param level updated kanji level
     * Is emitted when the current kanji compound of the detail page changes
     */
    void kanjiLevelChanged(const QString &level);
    /*!
     * \brief kanjiLastRepChanged signalizes when the compound repetition time changes
     * \param time updated last repetition time
     * Is emitted when the current kanji compound of the detail page changes
     */
    void kanjiLastRepChanged(const QString &time);

    /*!
     * \brief editPageRequested signalizes that an edit page should be opened
     * \param pageId id of the edit page (must be set externally in #editPageId)
     */
    void editPageRequested(int pageId);
    /*!
     * \brief kanjiDeletionRequested signalizes that the current kanji should be deleted
     * \param kanjiId id of the current kanji which should be deleted
     */
    void kanjiDeletionRequested(kcomp::kanji_id kanjiId);

public slots:
    /*!
     * \brief onKanjiChanged reflects change of the current compound
     * \param kc reference to the new current compound
     * Changes current compound of the detail to \p kc, emits
     * kanjiTextChanged(), kanjiReadingChanged(), kanjiMeaningChanged(),
     * kanjiLevelChanged(), kanjiLastRepChanged() as to update the detail page.
     */
    void onKanjiChanged(kcomp &kc);
    /*!
     * \brief onEditClicked requests edit page
     * Request edit page of the current kanji, emits editPageRequested()
     */
    void onEditClicked();
    /*!
     * \brief onDeletionClicked requests kanji deletion
     * Requests the deletion of the current kanji, emits kanjiDeletionRequested()
     */
    void onDeletionClicked();

private:
    /*!
     * \brief setupLayout sets up detail page layout
     */
    void setupLayout();
    /*!
     * \brief setupKanjiData sets up kanji data labels
     */
    void setupKanjiData();
    /*!
     * \brief setupButtons sets up edit and delete buttons
     */
    void setupButtons();

    QVBoxLayout *l;
    kcomp *curr_kanji; /*!< Currently displayed kanji compound*/

    Ui::DetailsWidget *ui;
};

#endif // DETAILSWIDGET_H

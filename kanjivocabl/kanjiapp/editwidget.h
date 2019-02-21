#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include "kanjiapp/KanjiData.h"

#include <QWidget>

#include <QVBoxLayout>
#include <QLineEdit>


namespace Ui {
class EditWidget;
}

/*!
 * \brief The EditWidget class represents a page for kanji compound editing
 */
class EditWidget : public QWidget
{
    Q_OBJECT
    using kcomp = kanji_data::kanji_compound;

public:
    explicit EditWidget(QWidget *parent = nullptr);
    virtual ~EditWidget() override;

signals:
    /*!
     * \brief kanjiTextChanged signalizes that current kanji characters changed
     * \param text new characters to be displayed
     */
    void kanjiTextChanged(const QString &text);
    /*!
     * \brief kanjiReadingChanged signalizes that current kanji reading changed
     * \param text new reading to be displayed
     */
    void kanjiReadingChanged(const QString &text);
    /*!
     * \brief kanjiMeaningChanged signalizes that current kanji meaning changed
     * \param text new meaning to be displayed
     */
    void kanjiMeaningChanged(const QString &text);

    /*!
     * \brief kanjiUpdateSaved submits the update
     * \param kanji updated kanji
     */
    void kanjiUpdateSaved(kcomp &kanji);

public slots:
    /*!
     * \brief onKanjiChanged updates page data according to the new kanji
     * \param kc new kanji to be updated
     * Emits kanjiTextChanged(), kanjiReadingChanged(), kanjiMeaningChanged()
     * to update page data.
     */
    void onKanjiChanged(kcomp &kc);
    /*!
     * \brief onSaveClicked requests save of the updated kanji
     */
    void onSaveClicked();

private:
    /*!
     * \brief setupLayout sets up the page layout
     */
    void setupLayout();

    /*!
     * \brief setupKanjiData sets up page fields
     */
    void setupKanjiData();
    /*!
     * \brief setupButtons sets up buttons
     */
    void setupButtons();

    QVBoxLayout *l;
    kcomp *curr_kanji; /*!< Currently displayed kanji compound*/

    QLineEdit *readingField;
    QLineEdit *meaningField;

    Ui::EditWidget *ui;
};

#endif // EDITWIDGET_H

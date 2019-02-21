#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QMap>

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>

namespace Ui {
class FilterDialog;
}

/*!
 * \brief The FilterDialog class represents a dialog to filter kanji compounds.
 */
class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    virtual ~FilterDialog() override;

    /*!
     * \brief The FilterMode enum represents different filter criteria.
     */
    enum class FilterMode {
        none, byKanji, byReading, byMeaning
    };

signals:
    /*!
     * \brief filterConfirmed signalizes that the filter should be applied
     * \param mode filter mode
     * \param param filter string if needed
     */
    void filterConfirmed(FilterMode mode, QString param);
    /*!
     * \brief enableFilterVal sets filter value field enabled or disabled
     * \param hasFilter enable/disable
     */
    void enableFilterVal(bool hasFilter);

private slots:
    /*!
     * \brief onFilterClicked is called when user confirms the filter dialog.
     */
    void onFilterClicked();
    /*!
     * \brief onResetClicked resets the filter
     * On reset, filterConfirmed is emitted with mode set to none
     */
    void onResetClicked();

private:
    /*!
     * \brief setupLayout sets up the layout
     */
    void setupLayout();
    /*!
     * \brief setupFilter sets up the filter page
     */
    void setupFilter();
    /*!
     * \brief setupButtons sets up the dialog buttons
     */
    void setupButtons();

    QGridLayout *l;
    QComboBox *cb;
    QLineEdit *filterVal;
    QDialogButtonBox *buttonBox;

    QMap<QString, FilterMode> modeMap; /*!< Maps filter strings to FilterMode. */

    FilterMode prevMode; /*!< Is used to determine whether the filter changed. */
    QString prevString; /*!< Is used to determine whether the filter string changed. */

    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H

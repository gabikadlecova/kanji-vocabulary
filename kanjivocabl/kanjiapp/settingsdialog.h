#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "kanjiapp/iolib.h"

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>

#include <QGridLayout>
#include <QSpinBox>

namespace Ui {
class SettingsDialog;
}

/*!
 * \brief The SettingsDialog class represents a dialog which changes app settings.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings s, QWidget *parent = nullptr);
    virtual ~SettingsDialog() override;

signals:
    /*!
     * \brief settingsSaveRequested Signalizes that the settings should be saved.
     * \param s Settings to be saved.
     * \param def If true, the settings will be saved to the default path.
     */
    void settingsSaveRequested(Settings s, bool def);
    /*!
     * \brief settingsLoadRequested Signalizes that new settings should be loaded.
     */
    void settingsLoadRequested();

public slots:
    /*!
     * \brief onSettingsLoaded Loads new settings to the dialog.
     * \param newSettings Settings which will be loaded.
     */
    void onSettingsLoaded(Settings newSettings);

private slots:
    /*!
     * \brief onSetDefClicked Is called when the user saves the settings to default path.
     */
    void onSetDefClicked();
    /*!
     * \brief onSaveClicked Is called when the user saves the settings to an arbitrary path.
     */
    void onSaveClicked();
    /*!
     * \brief onLoadClicked Is called when the user wants to load settings.
     */
    void onLoadClicked();

private:
    /*!
     * \brief setupLayout Sets up the layout.
     */
    void setupLayout();
    /*!
     * \brief setupContent Sets up the settings form.
     */
    void setupContent();
    /*!
     * \brief setupButtons Sets up the form buttons.
     */
    void setupButtons();

    QGridLayout *l;

    QSpinBox *kanjiPerRep;
    QSpinBox *maxLevel;
    QSpinBox *multiplier;

    QDialogButtonBox *buttonBox;

    Settings s;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H

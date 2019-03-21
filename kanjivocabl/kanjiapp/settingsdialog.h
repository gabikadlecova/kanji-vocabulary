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

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings s, QWidget *parent = nullptr);
    virtual ~SettingsDialog() override;

signals:
    void settingsSaveRequested(Settings s, bool def);
    void settingsLoadRequested();

public slots:
    void onSettingsLoaded(Settings newSettings);

private slots:
    void onSetDefClicked();
    void onSaveClicked();
    void onLoadClicked();

private:
    void setupLayout();
    void setupContent();
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

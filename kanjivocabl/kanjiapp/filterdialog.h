#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();

    enum class FilterMode {
        byKanji, byReading, byMeaning
    };

signals:
    void filterConfirmed(FilterMode mode, QString param);

private slots:
    void onFilterClicked();
    void onResetClicked();

private:
    // TODO size should be fixed (it's weird otherwise)
    // or change margins in setupFilter
    void setupLayout();
    void setupFilter();
    void setupButtons();

    QGridLayout *l;
    QComboBox *cb;
    QLineEdit *filterVal;
    QDialogButtonBox *box;

    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H

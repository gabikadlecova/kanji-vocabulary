#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QLineEdit>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT
    using kcomp = kanji_data::kanji_compound;

public:
    explicit EditWidget(QWidget *parent = nullptr);
    ~EditWidget();

signals:
    void kanjiTextChanged(const QString &text);
    void kanjiReadingChanged(const QString &text);
    void kanjiMeaningChanged(const QString &text);

    // TODO change reference to value and handle it in main for DetailsWidget!
    void kanjiUpdateSaved(kcomp &kanji);

public slots:
    void onKanjiChanged(kcomp &kc);
    void onSaveClicked();

private:
    void setupLayout();
    void setupKanjiData();
    void setupButtons();

    QVBoxLayout *l;
    kcomp *curr_kanji;

    QLineEdit *reading_l;
    QLineEdit *meaning_l;

    Ui::EditWidget *ui;
};

#endif // EDITWIDGET_H

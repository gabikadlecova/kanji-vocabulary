#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class AddKanjiWidget;
}

class AddKanjiWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

public:
    explicit AddKanjiWidget(QWidget *parent = nullptr);
    ~AddKanjiWidget();

signals:
    void kanjiAddRequested(kcomp kc);

private slots:
    void onAddClicked();

private:
    void setupLayout();
    void setupButtons();

    QVBoxLayout *l;

    QLineEdit *kanji_l;
    QLineEdit *reading_l;
    QLineEdit *meaning_l;

    Ui::AddKanjiWidget *ui;
};

#endif // ADDWIDGET_H

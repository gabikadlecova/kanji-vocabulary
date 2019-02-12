#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStack>
#include <QVector>
#include <QSplitter>
#include <QStackedWidget>

#include "kanjiwidget.h"
#include "kanjilistwidget.h"
#include "filterdialog.h"

#include "kanjiapp/KanjiData.h"


namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

public:
    explicit MainWidget(kanji_data::kanji_lib lib,
                        QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void pageOpened();
    void homeOpened();

    // todo emit some of this somewhere
    void kanjiAdded(kcomp kc);
    void kanjiDeleted(kcomp::kanji_id id);
    void kanjiChanged(kcomp kc);
    void kanjiFiltered(QVector<kcomp> newFilter);
    void filterReset();

private slots:
    void onPageChanged(int pageId);
    void onHomeButtonClicked();
    void onBackButtonClicked();

    void onKanjiDeleted(kcomp::kanji_id id);
    void onKanjiChanged(kcomp kc);
    void onKanjiAdded(kcomp kc);

    void onKanjiFiltered(FilterDialog::FilterMode fm, QString filterVal);

private:
    void setupLayout();
    void setupMenu();
    void setupPage();

    kanji_data::kanji_lib lib;
    QVector<kcomp> kanji_v;

    FilterDialog *d;
    QSplitter *menuSplitter;
    QStackedWidget *pageStack;
    QStack<int> idStack;

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStack>
#include <QVector>
#include <QSplitter>
#include <QStackedWidget>

#include "kanjiwidget.h"
#include "kanjilistwidget.h"
#include "kanjiapp/KanjiData.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(kanji_data::kanji_lib lib,
                        QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void pageOpened();
    void homeOpened();

    void kanjiAdded(kanji_data::kanji_compound kc);

private slots:
    void onPageChanged(int pageId);
    void onHomeButtonClicked();
    void onBackButtonClicked();

    void onKanjiDeleted(kanji_data::kanji_compound::kanji_id id);
    void onKanjiChanged(kanji_data::kanji_compound &kc);
    void onKanjiAdded(kanji_data::kanji_compound kc);

private:
    void setupLayout();
    void setupMenu();
    void setupPage();

    kanji_data::kanji_lib lib;
    QVector<kanji_data::kanji_compound> kanji_v;

    QSplitter *menuSplitter;
    QStackedWidget *pageStack;
    QStack<int> *idStack;

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

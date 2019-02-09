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
    explicit MainWidget(QVector<kanji_data::kanji_compound> kanji,
                        QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void page_opened();
    void home_opened();

private slots:
    void pageChanged(int pageId);
    void homeButtonClicked();
    void backButtonClicked();

private:
    void setupLayout();
    void setupMenu();
    void setupPage();

    QSplitter *menuSplitter;
    QStackedWidget *pageStack;
    KanjiListWidget *kanjiList;

    QStack<int> *idStack;
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

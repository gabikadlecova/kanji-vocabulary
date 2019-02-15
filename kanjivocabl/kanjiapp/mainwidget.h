#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QStack>
#include <QVector>
#include <QSplitter>
#include <QStackedWidget>

#include "kanjiapp/iolib.h"
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
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void pageOpened();
    void homeOpened();

    void kanjiLoaded(QVector<kcomp> kanji);

    void kanjiAdded(kcomp kc);
    void kanjiDeleted(kcomp::kanji_id id);
    void kanjiChanged(kcomp kc);
    void kanjiFiltered(QVector<kcomp> newFilter);
    void filterReset();
    void trainingDataChanged(std::vector<kcomp> trainKanji);

    void dataSaved(const kanji_data::kanji_lib &lib);

private slots:
    void onPageChanged(int pageId);
    void onHomeButtonClicked();
    void onBackButtonClicked();

    void onCustomMenu(QSplitter *newMenu);
    void onDefaultMenu();

    void onKanjiDeleted(kcomp::kanji_id id);
    void onKanjiChanged(kcomp kc);
    void onKanjiAdded(kcomp kc);

    void onKanjiFiltered(FilterDialog::FilterMode fm, QString filterVal);
    void onTrainingRequested();
    void onTrainingSubmitted(const std::vector<kcomp> &trainedKanji);
    void onTrainingFinished();

    void onLibLoaded(kanji_data::kanji_lib lib);
    void onLoadFailed();
    void onSaveFailed();

private:
    void setupLayout();
    void setupMenu();
    void setupPage();

    LibManip *lm;
    kanji_data::kanji_lib lib;

    FilterDialog *d;

    QVBoxLayout *l;
    QSplitter *menuSplitter;
    QSplitter *altMenuSplitter;

    QStackedWidget *pageStack;
    QStack<int> idStack;

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

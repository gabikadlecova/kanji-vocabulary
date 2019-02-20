#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "kanjiwidget.h"
#include "kanjilistwidget.h"
#include "filterdialog.h"

#include "kanjiapp/iolib.h"
#include "kanjiapp/KanjiData.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStack>
#include <QVector>
#include <QSplitter>
#include <QStackedWidget>

namespace Ui {
class MainWidget;
}

/*!
 * \brief The MainWidget class Connects all pages together and provides common handlers.
 */
class MainWidget : public QWidget {
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;
public:
    explicit MainWidget(std::string fileName = "", QWidget *parent = nullptr);
    ~MainWidget();

signals:
    /*!
     * \brief pageOpened Signalizes that a page from the #pageStack was shown.
     */
    void pageOpened();
    /*!
     * \brief pageNumberOpened Signalizes that a page with index \p pageNo was shown.
     * \param pageNo Index of the page in the #pageStack
     */
    void pageNumberOpened(int pageNo);

    /*!
     * \brief homeOpened Signalizes that the homepage (index 0) was shown.
     */
    void homeOpened();

    /*!
     * \brief kanjiLoaded Signalizes that new kanji data was loaded.
     * \param kanji Vector of kanji compounds to serve as the app model.
     */
    void kanjiLoaded(QVector<kcomp> kanji);
    /*!
     * \brief kanjiAdded Signalizes that a new kanji was added to the library.
     * \param kc Kanji compound that has been added.
     */
    void kanjiAdded(kcomp kc);
    /*!
     * \brief kanjiDeleted Signalizes that a kanji was deleted.
     * \param id Id of the deleted kanji compound.
     */
    void kanjiDeleted(kcomp::kanji_id id);
    /*!
     * \brief kanjiChanged Signalizes that a particular kanji compound changed.
     * \param kc Modified kanji compound.
     */
    void kanjiChanged(kcomp kc);

    /*!
     * \brief kanjiFiltered Signalizes that a filter has been applied.
     * \param newFilter Filtered data.
     */
    void kanjiFiltered(QVector<kcomp> newFilter);
    /*!
     * \brief filterReset Signalizes that the filter has been reset to default.
     */
    void filterReset();

    /*!
     * \brief trainingDataChanged Provides training data.
     * \param trainKanji Kanji compounds which are to be trained.
     */
    void trainingDataChanged(std::vector<kcomp> trainKanji);

    /*!
     * \brief dataSaved Signalizes that the library should be saved.
     * \param lib Library that is to be saved.
     */
    void dataSaved(const kanji_data::kanji_lib &lib);

private slots:
    /*!
     * \brief onPageChanged Is called when the current page changes.
     * \param pageId Id of the new page shown.
     */
    void onPageChanged(int pageId);
    /*!
     * \brief onHomeButtonClicked Is called when the home menu button is clicked.
     */
    void onHomeButtonClicked();
    /*!
     * \brief onBackButtonClicked Is called when the back menu button is clicked.
     */
    void onBackButtonClicked();

    /*!
     * \brief onCustomMenu Replaces temporarily the default menu with a custom menu.
     * \param newMenu Custom menu to be shown.
     */
    void onCustomMenu(QSplitter *newMenu);
    /*!
     * \brief onDefaultMenu Shows the default menu and possibly hides a custom menu.
     */
    void onDefaultMenu();

    /*!
     * \brief onKanjiDeleted Is called if a kanji compound is to be deleted.
     * \param id Id of the compound to be deleted.
     */
    void onKanjiDeleted(kcomp::kanji_id id);
    /*!
     * \brief onKanjiChanged Is called if a kanji compound is to be updated.
     * \param id Kanji compound to be updated.
     */
    void onKanjiChanged(kcomp kc);
    /*!
     * \brief onKanjiAdded Is called if a kanji compound is to be added.
     * \param id Kanji compound to be added.
     */
    void onKanjiAdded(kcomp kc);

    /*!
     * \brief onKanjiFiltered Is called if the kanji data should be filtered.
     * \param fm
     * \param filterVal
     */
    void onKanjiFiltered(FilterDialog::FilterMode fm, QString filterVal);
    void onTrainingRequested();
    void onTrainingSubmitted(const std::vector<kcomp> &trainedKanji);
    void onTrainingFinished();

    void onLibLoaded(kanji_data::kanji_lib lib);
    void onBadFormat();
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
    QLabel *title;
    QSplitter *menuSplitter;
    QSplitter *altMenuSplitter;

    QStackedWidget *pageStack;
    QStack<int> idStack;

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

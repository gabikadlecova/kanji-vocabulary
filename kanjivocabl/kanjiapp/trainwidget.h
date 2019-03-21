#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include "kanjiapp/KanjiData.h"
#include "kanjiapp/iolib.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <set>

namespace Ui {
class TrainWidget;
}

/*!
 * \brief The TrainWidget class represents a flashcard training page.
 */
class TrainWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

    /*!
     * \brief The FlipResponse enum Represents the result of the training of a compound.
     */
    enum class FlipResponse {
        yes, maybe, no
    };

public:
    explicit TrainWidget(Settings s, QWidget *parent = nullptr);
    virtual ~TrainWidget() override;

signals:
    /*!
     * \brief customMenuShown Signalizes that the page uses its own menu.
     * \param menu Custom page menu.
     */
    void customMenuShown(QSplitter *menu);
    /*!
     * \brief customMenuHidden Signalizes that the custom menu is not needed anymore.
     */
    void customMenuHidden();

    // used in this widget
    /*!
     * \brief kanjiChanged Signalizes when compound kanji characters change
     * \param k updated kanji characters
     * Is emitted when the current kanji compound of the train page changes
     */
    void kanjiChanged(const QString &k);
    /*!
     * \brief kanjiReadingChanged Signalizes when the compound reading changes
     * \param r updated kanji reading
     * Is emitted when the current kanji compound of the train page changes
     */
    void kanjiReadingChanged(const QString &r);
    /*!
     * \brief kanjiMeaningChanged Signalizes when the compound meaning changes
     * \param m updated kanji meaning
     * Is emitted when the current kanji compound of the train page changes
     */
    void kanjiMeaningChanged(const QString &m);
    /*!
     * \brief kanjiRemainingChanged Signalizes when the number of remaning kanji changes.
     * \param rem Updated count of remaining compounds
     * Is emitted when the current kanji compound is repeated with "yes" or "no",
     * which means that the level changes; "not sure" means skip.
     */
    void kanjiRemainingChanged(const QString &rem);

    // need to be connected
    /*!
     * \brief trainingEnded Ends the current training and submits the results.
     * \param result Repeated kanji.
     */
    void trainingEnded(const QVector<kcomp> &result);
    /*!
     * \brief trainingDiscarded Ends the current training and discards all changes.
     */
    void trainingDiscarded();

public slots:
    /*!
     * \brief onTrainKanjiSet Loads a new training set of compounds.
     * \param newTraining New compounds to be trained.
     */
    void onTrainKanjiSet(QVector<kcomp> newTraining);

    void onSettingsLoaded(Settings s);

private slots:
    /*!
     * \brief onFlipClicked Is called when user flips the kanji flashcard.
     */
    void onFlipClicked();
    /*!
     * \brief onResponseSelected Is called when the user selects a training response.
     * \param fr Selected training response.
     */
    void onResponseSelected(FlipResponse fr);
    /*!
     * \brief onTrainingEndClicked Is called when the user clicks the end button.
     * Tries to end the training, offers a dialog to the user. The user can decide,
     * whether they want to save or discard changes, or not to end the training.
     */
    void onTrainingEndClicked();
    /*!
     * \brief onBackButtonClicked Is called when the user clicks the back button.
     * It either flips back a flipped flashcard, or it goes to the previous kanji
     * compound and discards any changes made to it.
     */
    void onBackButtonClicked();

private:
    virtual void showEvent(QShowEvent *e) override;
    virtual void hideEvent(QHideEvent *e) override;

    /*!
     * \brief setupLayout Sets up train page layout, both sides of the flashcard.
     */
    void setupLayout();
    /*!
     * \brief setupMenuButtons Sets up the custom menu.
     */
    void setupMenuButtons();
    /*!
     * \brief setupTrainPage Sets up the train page of the flashcard (not flipped).
     */
    void setupTrainPage();
    /*!
     * \brief setupFlippedPage Sets up the flipped page of the flashcard.
     */
    void setupFlippedPage();
    /*!
     * \brief setupButtons Sets up response buttons
     */
    void setupButtons();

    /*!
     * \brief updateRemainingLabel Updates a label which shows how many compounds remain.
     */
    void updateRemainingLabel();
    /*!
     * \brief updateKanjiLabels Updates the displayed kanji compound.
     */
    void updateKanjiLabels();
    /*!
     * \brief newCycle Starts a new repetition cycle.
     * The method either starts a new repetition cycle (for example after having
     * responded to all kanji, or as a first cycle), or ends the training when
     * no compounds remain. The latter occurs when there are no compounds available
     * or when all compounds had been trained as "yes" or "no" in the previous cycle.
     */
    void newCycle();
    /*!
     * \brief flipBack Flips back the flashcard without affecting any stats or display.
     */
    void flipBack();

    QVBoxLayout *l;
    QSplitter *menu;

    QLabel *kanjiText;
    QLabel *kanjiReading;
    QLabel *kanjiMeaning;

    QPushButton *flipButton; /*!< Flips the flashcard. */
    QSplitter *feedbackSplitter; /*!< Training buttons for selecting a response. */

    QVector<kcomp> trainKanji; /*!< Unchanged input kanji data. */
    QVector<kcomp> resKanji; /*!< Result kanji; all changes are made here. */
    QVector<kcomp::kanji_id> history; /*!< Repetition history; enables "go back". */
    std::set<kcomp::kanji_id> validId; /*!< Kanji compounds which need to be repeated. */

    QVector<kcomp>::iterator currKanji;

    bool flipped = false; /*!< Indicates whether the flashcard is flipped. */

    Settings s;

    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

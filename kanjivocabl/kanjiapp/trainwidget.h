#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <set>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class TrainWidget;
}

class TrainWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;
    enum class FlipResponse {
        yes, maybe, no
    };

public:
    explicit TrainWidget(QWidget *parent = nullptr);
    virtual ~TrainWidget() override;

signals:
    void customMenuShown(QSplitter *menu);
    void customMenuHidden();

    // used in this widget
    void kanjiChanged(const QString &k);
    void kanjiMeaningChanged(const QString &m);
    void kanjiReadingChanged(const QString &r);
    void kanjiRemainingChanged(const QString &rem);

    // need to be connected
    void trainingEnded(const std::vector<kcomp> &result);
    void trainingDiscarded();

public slots:
    void onTrainKanjiSet(std::vector<kcomp> newTraining);

private slots:
    void onFlipClicked();
    void onResponseSelected(FlipResponse fr);
    void onTrainingEndClicked();
    void onBackButtonClicked();

private:
    virtual void showEvent(QShowEvent *e) override;
    virtual void hideEvent(QHideEvent *e) override;

    void setupLayout();
    void setupMenuButtons();
    void setupTrainPage();
    void setupFlippedPage();
    void setupButtons();

    void updateRemainingLabel();
    void updateKanjiLabels();
    void newCycle();
    void flipBack();

    QVBoxLayout *l;
    QSplitter *menu;
    QLabel *kanjiText;

    QLabel *kanjiReading;
    QLabel *kanjiMeaning;

    QPushButton *flipButton;
    QSplitter *feedbackSplitter;

    std::vector<kcomp> trainKanji;
    std::vector<kcomp> resKanji;
    std::vector<kcomp::kanji_id> history;
    std::set<kcomp::kanji_id> validId;

    std::vector<kcomp>::iterator currKanji;

    bool flipped = false;

    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <stack>

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

    void trainingStarted();
    void trainingEnded(const std::vector<kcomp> &result);
    void trainingDiscarded();

public slots:
    void onTrainKanjiSet(std::vector<kcomp> newTraining);

private slots:
    void onFlipClicked();
    void onResponseSelected(FlipResponse fr);

private:
    virtual void showEvent(QShowEvent *e) override;
    virtual void hideEvent(QHideEvent *e) override;

    void setupLayout();
    void setupButtons();
    void setupTrainPage();

    QVBoxLayout *l;
    QSplitter *menu;
    QLabel *kanjiText;

    QPushButton *flipButton;
    QSplitter *feedbackSplitter;

    std::vector<kcomp> trainKanji;
    std::vector<kcomp::kanji_id> history;
    std::vector<kcomp::kanji_id> validId;

    std::vector<kcomp>::iterator currKanji;

    bool flipped = false;

    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>

#include <vector>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class TrainWidget;
}

class TrainWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

public:
    explicit TrainWidget(QWidget *parent = nullptr);
    virtual ~TrainWidget() override;
    int pageId;

signals:
    void customMenuShown(QSplitter *menu);
    void customMenuHidden();

    void trainingEnded();

public slots:
    void onTrainKanjiSet(std::vector<kcomp> newTraining);

private:
    virtual void showEvent(QShowEvent *e) override;
    virtual void hideEvent(QHideEvent *e) override;

    void setupLayout();
    void setupButtons();
    void setupTrainPage();

    QVBoxLayout *l;
    QSplitter *menu;
    QLabel *kanjiText;

    std::vector<kcomp> trainKanji;

    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>
#include <QGridLayout>
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
    ~TrainWidget();

public slots:
    void setTrainKanji(std::vector<kcomp> newTraining);

private:
    void setupLayout();
    void setupButtons();
    void setupTrainPage();

    QGridLayout *l;

    std::vector<kcomp> trainKanji;

    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

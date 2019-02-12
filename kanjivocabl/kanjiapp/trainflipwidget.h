#ifndef TRAINFLIPWIDGET_H
#define TRAINFLIPWIDGET_H

#include <QWidget>

namespace Ui {
class TrainFlipWidget;
}

class TrainFlipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainFlipWidget(QWidget *parent = nullptr);
    ~TrainFlipWidget();

private:
    Ui::TrainFlipWidget *ui;
};

#endif // TRAINFLIPWIDGET_H

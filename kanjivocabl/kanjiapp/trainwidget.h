#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>

namespace Ui {
class TrainWidget;
}

class TrainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrainWidget(QWidget *parent = nullptr);
    ~TrainWidget();

private slots:
    void on_flip_button_clicked();

private:
    Ui::TrainWidget *ui;
};

#endif // TRAINWIDGET_H

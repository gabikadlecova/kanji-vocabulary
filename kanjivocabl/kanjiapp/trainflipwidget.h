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

private slots:
    void on_ok_button_clicked();

    void on_maybe_button_clicked();

    void on_no_button_clicked();

private:
    Ui::TrainFlipWidget *ui;
};

#endif // TRAINFLIPWIDGET_H

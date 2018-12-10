#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QWidget>

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = nullptr);
    ~EditWidget();

private slots:
    void on_confirm_button_clicked();

private:
    Ui::EditWidget *ui;
};

#endif // EDITWIDGET_H

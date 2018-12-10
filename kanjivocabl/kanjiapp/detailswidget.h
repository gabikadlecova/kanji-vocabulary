#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsWidget(QWidget *parent = nullptr);
    ~DetailsWidget();

private slots:
    void on_edit_button_clicked();

    void on_reset_button_clicked();

private:
    Ui::DetailsWidget *ui;
};

#endif // DETAILSWIDGET_H

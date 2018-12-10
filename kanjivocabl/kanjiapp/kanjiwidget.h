#ifndef KANJIWIDGET_H
#define KANJIWIDGET_H

#include <QWidget>

namespace Ui {
class KanjiWidget;
}

class KanjiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanjiWidget(QWidget *parent = nullptr);
    ~KanjiWidget();

private slots:
    void on_train_button_clicked();

    void on_list_button_clicked();

    void on_pushButton_clicked();


private:
    Ui::KanjiWidget *ui;
};

#endif // KANJIWIDGET_H

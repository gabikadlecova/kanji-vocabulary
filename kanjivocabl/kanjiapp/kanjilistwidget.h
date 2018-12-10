#ifndef KANJILISTWIDGET_H
#define KANJILISTWIDGET_H

#include <QWidget>

namespace Ui {
class KanjiListWidget;
}

class KanjiListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanjiListWidget(QWidget *parent = nullptr);
    ~KanjiListWidget();

private:
    Ui::KanjiListWidget *ui;
};

#endif // KANJILISTWIDGET_H

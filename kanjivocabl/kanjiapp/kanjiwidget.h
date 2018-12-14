#ifndef KANJIWIDGET_H
#define KANJIWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>

#include <string>

namespace Ui {
class KanjiWidget;
}

class KanjiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanjiWidget(QStackedWidget *st, QWidget *parent = nullptr);
    ~KanjiWidget();

    int addWidget(const QString &name, QWidget *widget, const QIcon *icon = nullptr);

signals:
    void pageButtonClicked(int buttonId);

private:
    void setupLayout();

    int widgetCount;
    QGridLayout *l;
    QStackedWidget *pageStack;

    Ui::KanjiWidget *ui;
};

#endif // KANJIWIDGET_H

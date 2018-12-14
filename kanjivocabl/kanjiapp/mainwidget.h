#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QStackedWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

signals:
    void page_opened();
    void home_opened();

private slots:
    void pageChanged(int pageId);
    void homeButtonClicked();
    void backButtonClicked();

private:
    void setupLayout();
    void setupMenu();
    void setupPage();

    QSplitter *menuSplitter;
    QStackedWidget *pageStack;

    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

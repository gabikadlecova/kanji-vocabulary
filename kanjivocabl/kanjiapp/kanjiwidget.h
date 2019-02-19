#ifndef KANJIWIDGET_H
#define KANJIWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPushButton>


namespace Ui {
class KanjiWidget;
}

/*!
 * \brief The KanjiWidget class represents the title page of the application.
 */
class KanjiWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanjiWidget(QStackedWidget *st, QWidget *parent = nullptr);
    ~KanjiWidget();

    /*!
     * \brief addWidget Adds a widget to the menu
     * \param name Button name to be displayed
     * \param widget Widged which will be displayed if the user selects its button
     * \param icon Button icon
     * \return Index of the widget in the #pageStack
     */
    int addWidget(const QString &name, QWidget *widget, const QIcon *icon = nullptr);
    /*!
     * \brief addButton Adds a simple button to the menu
     * \param button Button to be added
     */
    void addButton(QPushButton *button);

signals:
    /*!
     * \brief pageButtonClicked Is emitted if the user select a button
     * \param buttonId Id of the button
     */
    void pageButtonClicked(int buttonId);

private:
    void setupLayout();

    int widgetCount; /*!< Is used to determine the position of new buttons. */
    QGridLayout *l;
    QStackedWidget *pageStack;

    Ui::KanjiWidget *ui;
};

#endif // KANJIWIDGET_H

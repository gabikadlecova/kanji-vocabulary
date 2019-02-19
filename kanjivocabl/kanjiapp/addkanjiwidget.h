#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include "kanjiapp/KanjiData.h"

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>

namespace Ui {
class AddKanjiWidget;
}

/*!
 * \brief The AddKanjiWidget class represents a page for adding a new kanji compound.
 */
class AddKanjiWidget : public QWidget
{
    Q_OBJECT

    using kcomp = kanji_data::kanji_compound;

public:
    explicit AddKanjiWidget(QWidget *parent = nullptr);
    virtual ~AddKanjiWidget() override;

signals:
    /*!
     * \brief kanjiAddRequested is emitted if the new compound \p kc should be added.
     * \param kc compound to be added.
     */
    void kanjiAddRequested(kcomp kc);

public slots:
    /*!
     * \brief onAddSucceeded on success, all fields should be emptied
     * \param kc is not used
     */
    void onAddSucceeded(kcomp kc);

private slots:
    /*!
     * \brief onAddClicked is called when the user confirms the form
     */
    void onAddClicked();

private:
    /*!
     * \brief setupLayout setups all fields and buttons
     */
    void setupLayout();
    /*!
     * \brief setupButtons setups the add button
     */
    void setupButtons();

    QVBoxLayout *l;

    QLineEdit *kanjiField;
    QLineEdit *readingField;
    QLineEdit *meaningField;

    Ui::AddKanjiWidget *ui;
};

#endif // ADDWIDGET_H

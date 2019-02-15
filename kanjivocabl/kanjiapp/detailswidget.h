#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "kanjiapp/KanjiData.h"

namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT
    using kcomp = kanji_data::kanji_compound;

public:
    explicit DetailsWidget(QWidget *parent = nullptr);
    ~DetailsWidget();
    int editPageId;

signals:
    void kanjiTextChanged(const QString &text);
    void kanjiReadingChanged(const QString &text);
    void kanjiMeaningChanged(const QString &text);
    void kanjiLevelChanged(kcomp::kanji_level level);
    void kanjiLastRepChanged(const QString &time);

    void editPageRequested(int pageId);
    void kanjiDeletionRequested(kcomp::kanji_id kanjiId);

public slots:
    void onKanjiChanged(kcomp &kc);
    void onEditClicked();
    void onDeletionClicked();

private:
    void setupLayout();
    void setupKanjiData();
    void setupButtons();

    QVBoxLayout *l;
    kcomp *curr_kanji;

    Ui::DetailsWidget *ui;
};

#endif // DETAILSWIDGET_H

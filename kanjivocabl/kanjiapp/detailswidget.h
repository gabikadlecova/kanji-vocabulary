#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>
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

signals:
    void kanjiTextChanged(const QString &text);
    void kanjiReadingChanged(const QString &text);
    void kanjiMeaningChanged(const QString &text);
    void kanjiLevelChanged(kcomp::kanji_level level);

public slots:
    void onKanjiChanged(kcomp &kc);

private:
    void setupLayout();

    kcomp *curr_kanji;

    Ui::DetailsWidget *ui;
};

#endif // DETAILSWIDGET_H

#ifndef KANJIFILTER_H
#define KANJIFILTER_H

#include "kanjiapp/KanjiData.h"
#include "filterdialog.h"

#include <QVector>

/*!
 * \brief get_by_kanji Creates a QVector of compounds which contain the given substring.
 * \param lib Library to be searched.
 * \param mode Filter mode to be applied.
 * \param val Filter parameter.
 * \return A new QVector of filtered kanji.
 */
QVector<kanji_data::kanji_compound> get_filtered(const kanji_data::kanji_lib &lib,
                                                 FilterDialog::FilterMode mode,
                                                 const QString &val);

/*!
 * \brief get_by_kanji Creates a QVector of compounds which are can be trained today.
 * \param lib Library to be searched.
 * \return A new QVector of filtered kanji.
 */
QVector<kanji_data::kanji_compound> get_due_today(kanji_data::kanji_lib &lib,
                                                  int maxCount = -1);


#endif // KANJIFILTER_H

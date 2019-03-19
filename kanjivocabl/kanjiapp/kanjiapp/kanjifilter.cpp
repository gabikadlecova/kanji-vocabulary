#include "kanjifilter.h"

QVector<kanji_data::kanji_compound> get_filtered(const kanji_data::kanji_lib &lib,
                                                 FilterDialog::FilterMode mode,
                                                 const QString &val)
{
    QVector<kanji_data::kanji_compound> res;
    std::wstring wval = val.toStdWString();

    auto kanji = lib.get_kanji();
    auto it = kanji.cbegin();

    using Mode = FilterDialog::FilterMode;
    while (true) {
        switch (mode) {
            case Mode::byKanji:
                it = kanji_data::by_kanji(it, kanji.cend(), wval);
                break;

            case Mode::byMeaning:
                it = kanji_data::by_meaning(it, kanji.cend(), wval);
                break;

            case Mode::byReading:
                it = kanji_data::by_reading(it, kanji.cend(), wval);
                break;

            default:
                throw std::logic_error("Invalid filter mode.");
                break;
        }

        if (it == kanji.cend()) {
            break;
        }

        res.push_back(*it);
    }

    return res;
}


QVector<kanji_data::kanji_compound> get_due_today(const kanji_data::kanji_lib &lib)
{
    QVector<kanji_data::kanji_compound> res;

    auto kanji = lib.get_kanji();
    auto it = kanji.cbegin();

    while (true) {
        it = kanji_data::due_today(it, kanji.cend());

        if (it == kanji.cend()) {
            break;
        }

        res.push_back(*it);
    }

    return res;
}


#include "kanjifilter.h"


// filters the kanji library
QVector<kanji_data::kanji_compound> get_filtered(const kanji_data::kanji_lib &lib,
                                                 FilterDialog::FilterMode mode,
                                                 const QString &val)
{
    QVector<kanji_data::kanji_compound> res;
    std::wstring wval = val.toStdWString();

    auto kanji = lib.get_kanji();
    auto it = kanji.cbegin();

    // retrieve compounds according to the filter
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

        // all compounds filtered
        if (it == kanji.cend()) {
            break;
        }

        // add next compound
        res.push_back(*it);
        it++;
    }

    return res;
}


// get compounds which can be repeated today
QVector<kanji_data::kanji_compound> get_due_today(kanji_data::kanji_lib &lib,
                                                  int maxCount)
{
    QVector<kanji_data::kanji_compound> res;

    auto kanji = lib.get_kanji();

    auto it = kanji.cbegin();

    // find all kanji which can be repeated
    while (true) {
        it = kanji_data::due_today(it, kanji.cend());

        if (it == kanji.cend()) {
            break;
        }

        res.push_back(*it);
        it++;
    }

    // get up to maxCount compounds
    std::sort(res.begin(), res.end(),
              [](kanji_data::kanji_compound kcl, kanji_data::kanji_compound kcr){
        return kcl.get_level() < kcr.get_level();
    });

    if (maxCount < 0 || maxCount >= res.size()) {
        return res;
    }

    return res.mid(0, maxCount);
}


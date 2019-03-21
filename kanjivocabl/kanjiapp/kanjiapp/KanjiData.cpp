#include "KanjiData.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>

#include <cstdio>
#include <iostream>

namespace kanji_data {

    // changes kanji level and time
    void kanji_compound::repeat(bool succeeded, kanji_level max_level,
                                std::int_least8_t multiplier) {
        // sets time to "now"
        last_rep_ = std::chrono::system_clock::now();

        // increases the repetition level or resets it
        if (succeeded) {
            if (level_ >= max_level) {
                return;
            }
            level_ = level_ * multiplier;
        }
        else {
            // repeat as soon as possible
            level_ = 1;
        }
    }


    // resets time to "now" without repeating (changing level)
    void kanji_compound::reset_time() {
        last_rep_ = std::chrono::system_clock::now();
    }


    // resets time of all kanji compounds in the library
    void kanji_lib::reset_time() {
        std::for_each(kanji_.begin(), kanji_.end(), [](kanji_compound kc) {
            kc.reset_time();
        });
    }


    // adds a kanji to the library, with checking for duplicates
    kanji_compound kanji_lib::add_kanji(std::wstring kanji_str, std::wstring reading,
                                        std::wstring meaning) {

        // if duplicate is found, add fails
        auto find_it = std::find_if(kanji_.begin(), kanji_.end(),
                                    [&](const kanji_compound &kc) {
            return kanji_str == kc.get_kanji();
        });

        if (find_it != kanji_.end()) {
            throw duplicate_error("Duplicate kanji");
        }

        // sets level to 1, sets a unique id, compound is handled as being last repeated
        // on addition
        kanji_compound kc{ std::move(kanji_str), std::move(reading), std::move(meaning),
                           ++max_id_, 1, std::chrono::system_clock::now() };
        kanji_.push_back(kc);

        return kc;
    }


    // updates the compound, checks for id validity
    void kanji_lib::update_kanji(kanji_compound kanji) {
        // kanji must be found by id
        auto it = std::find_if(kanji_.begin(), kanji_.end(),
                               [&](const kanji_compound &kc) {
            return kanji.get_id() == kc.get_id();
        });

        if (!kanji.id_valid() || it == kanji_.end()) {
            throw missing_error("Cannot update kanji - missing value");
        }

        // kanji data is entirely replaced, id remains the same
        *it = std::move(kanji);
    }

    // delete kanji compound with the given id. Does not delete or throw if
    // the id is not present
    void kanji_lib::delete_kanji(kanji_compound::kanji_id id) {
        kanji_.erase(std::remove_if(kanji_.begin(), kanji_.end(),
                                    [&](const kanji_compound &kc) {
            return kc.get_id() == id;
        }), kanji_.end());
    }

    // filters by kanji characters
    const_kanji_it by_kanji(const_kanji_it begin, const_kanji_it end,
                            const std::wstring &k) {

        auto it = std::find_if(begin, end,
                    [&](const kanji_compound &kc) {
            return kc.get_kanji().find(k) != std::wstring::npos;
        });

        return it;
    }

    // filters by meaning
    const_kanji_it by_meaning(const_kanji_it begin, const_kanji_it end,
                                           const std::wstring &m) {

        auto it = std::find_if(begin, end,
                     [&](const kanji_compound &kc) {
            return kc.meaning.find(m) != std::wstring::npos;
        });

        return it;
    }

    // filters by reading
    const_kanji_it by_reading(const_kanji_it begin, const_kanji_it end,
                                           const std::wstring &r) {

        auto it = std::find_if(begin, end,
                     [&](const kanji_compound &kc) {
            return kc.reading.find(r) != std::wstring::npos;
        });

        return it;
    }

    // compounds which can be repeated "now"
    const_kanji_it due_today(const_kanji_it begin, const_kanji_it end) {

        auto it = std::find_if(begin, end,
                               [&](const kanji_compound &kc) {

            // last repetition must have been at least kc.get_level() days ago
            time t = kc.get_last_rep() + std::chrono::hours(kc.get_level() * 24);

            return t <= std::chrono::system_clock::now();
        });

        return it;
    }

    // reads library from the input stream
    kanji_lib read_lib(std::wistream & is) {
        std::vector<kanji_compound> kanji; // result kanji
        kanji_compound::kanji_id id = 0; // id to be assigned to new kanji

        while (true) {
            if (!is.good()) {
                break;
            }

            std::wstring line;
            getline(is, line);

            // a line represents one compound
            if (!line.empty())
            {
                std::wstringstream ss{ line };

                std::wstring k; // kanji
                std::wstring r; // reading
                std::wstring m; // meaning

                time_t t; // last repetition time

                getline(ss, k, L';');
                getline(ss, r, L';');
                getline(ss, m, L';');

                std::wstring ts;
                getline(ss, ts, L';');
                std::wstringstream tss{ ts };
                tss >> t;

                std::wstring ls; // level
                getline(ss, ls, L';');

                // check if everything has been parsed correctly
                bool failed =
                        !std::all_of(ls.begin(), ls.end(), iswdigit) ||
                        !std::all_of(ts.begin(), ts.end(), iswdigit) ||
                        k == L"" ||
                        r == L"" ||
                        m == L"";

                if (failed) {
                    throw format_error("Could not parse input file.");
                }

                // add a new compound with a unique id
                kanji_compound kc{ k, r, m, ++id, std::stoi(ls),
                            std::chrono::system_clock::from_time_t(t) };
                kanji.push_back(std::move(kc));
            }
        }

        // return compounds along with id (maximum of all ids)
        return kanji_lib{ std::move(kanji), id };
    }

    // creates a lib with no compounds
    kanji_lib empty_lib()
    {
        return kanji_lib{ std::vector<kanji_compound>(), 0};
    }

    void write_kanji(const kanji_compound &kc, std::wostream &wos);

    // writes library to an output stream
    void write_lib(const kanji_lib &lib, std::wostream &wos)
    {
        std::for_each(lib.get_kanji().begin(), lib.get_kanji().end(),
                      [&wos](const kanji_compound &kc) {
            // one line per a compound
            write_kanji(kc, wos);
            wos << std::endl;
        });
    }

    // writes a single compound to the output stream
    void write_kanji(const kanji_compound &kc, std::wostream &wos)
    {
        // properties are separated by L';'
        wos << kc.get_kanji() << L';';
        wos << kc.reading << L';';
        wos << kc.meaning << L';';
        wos << std::chrono::system_clock::to_time_t(kc.get_last_rep()) << L';';
        wos << kc.get_level();
    }

}

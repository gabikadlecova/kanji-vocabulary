#include "KanjiData.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>

#include <cstdio>
#include <iostream>

void kanji_data::kanji_compound::repeat(bool succeeded) {
	last_rep_ = std::chrono::system_clock::now();

	if (succeeded) {
		if (level_ == max_level) {
			return;
		}
		level_ = level_ * 2;
	}
	else {
		if (level_ == 1) {
			return;
		}
		level_ = level_ / 2;
	}
}

void kanji_data::kanji_compound::reset_time() {
	last_rep_ = std::chrono::system_clock::now();
}

void kanji_data::kanji_lib::reset_time() {
	std::for_each(kanji_.begin(), kanji_.end(), [](kanji_compound kc) {kc.reset_time(); });
}

kanji_data::kanji_compound kanji_data::kanji_lib::add_kanji(std::wstring kanji_str, std::wstring reading, std::wstring meaning) {
	if (std::find_if(kanji_.begin(), kanji_.end(), [&](const kanji_compound &kc) {return kanji_str == kc.get_kanji(); }) != kanji_.end()) {
		throw std::logic_error("Duplicate kanji");
	}

	kanji_compound kc{ std::move(kanji_str), std::move(reading), std::move(meaning), ++max_id_, 1, std::chrono::system_clock::now() };
    kanji_.push_back(kc);

    return kc;
}

void kanji_data::kanji_lib::update_kanji(kanji_compound kanji) {
	auto it = std::find_if(kanji_.begin(), kanji_.end(), [&](const kanji_compound &kc) {return kanji.get_id() == kc.get_id(); });
	if (!kanji.id_valid() || it == kanji_.end()) {
		throw std::logic_error("Cannot update kanji - missing value");
	}

    *it = std::move(kanji);
}

void kanji_data::kanji_lib::delete_kanji(kanji_compound::kanji_id id) {
	kanji_.erase(std::remove_if(kanji_.begin(), kanji_.end(), [&](const kanji_compound &kc) {return kc.get_id() == id; }), kanji_.end());
}

std::vector<kanji_data::kanji_compound> kanji_data::by_kanji(const kanji_lib &lib, const std::wstring &k) {
	std::vector<kanji_compound> kanji_k;
    std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(),
                 std::back_inserter(kanji_k),
                 [&](const kanji_compound &kc) {
        return kc.get_kanji().find(k) != std::wstring::npos;
    });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::by_meaning(const kanji_lib &lib, const std::wstring &m) {
	std::vector<kanji_compound> kanji_k;
    std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(),
                 std::back_inserter(kanji_k),
                 [&](const kanji_compound &kc) {
        return kc.meaning.find(m) != std::wstring::npos;
    });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::by_reading(const kanji_lib &lib, const std::wstring &r) {
	std::vector<kanji_compound> kanji_k;
    std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(),
                 std::back_inserter(kanji_k),
                 [&](const kanji_compound &kc) {
        return kc.reading.find(r) != std::wstring::npos;
    });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::due_today(const kanji_lib &lib) {
	std::vector<kanji_compound> kanji_k;
	std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(), std::back_inserter(kanji_k), [&](const kanji_compound &kc) {
		time t = kc.get_last_rep() + std::chrono::hours(kc.get_level() * 24);
		return t <= std::chrono::system_clock::now();
	});

	return kanji_k;
}

kanji_data::kanji_lib kanji_data::read_lib(std::wistream & is) {
	std::vector<kanji_compound> kanji;
	kanji_compound::kanji_id id = 0;

	while (true) {
		if (!is.good()) {
			break;
		}

		std::wstring line;
		getline(is, line);

		if (!line.empty())
		{
			std::wstringstream ss{ line };

			std::wstring k;
			std::wstring r;
			std::wstring m;

			time_t t;

			getline(ss, k, L';');
			getline(ss, r, L';');
			getline(ss, m, L';');

			std::wstring ts;
			getline(ss, ts, L';');
			std::wstringstream tss{ ts };
			tss >> t;

			std::wstring ls;
			getline(ss, ls, L';');

			kanji_compound kc{ k, r, m, ++id, std::stoi(ls), std::chrono::system_clock::from_time_t(t) };
			kanji.push_back(std::move(kc));
		}
	}

	return kanji_lib{ std::move(kanji), id };
}

void write_kanji(const kanji_data::kanji_compound &kc, std::wostream &wos);

void kanji_data::write_lib(const kanji_lib &lib, std::wostream &wos)
{
	std::for_each(lib.get_kanji().begin(), lib.get_kanji().end(), [&wos](const kanji_compound &kc)
	{
		write_kanji(kc, wos);
		wos << std::endl;
	});
}

void write_kanji(const kanji_data::kanji_compound &kc, std::wostream &wos)
{
	wos << kc.get_kanji() << L";";
	wos << kc.reading << L";";
	wos << kc.meaning << L";";
	wos << std::chrono::system_clock::to_time_t(kc.get_last_rep()) << L";";
	wos << kc.get_level();
}

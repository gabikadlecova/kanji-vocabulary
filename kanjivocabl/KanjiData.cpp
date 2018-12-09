#include "KanjiData.h"

#include <string>
#include <algorithm>
#include <stdexcept>

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

void kanji_data::kanji_lib::add_kanji(std::wstring kanji_str, std::string reading, std::string meaning) {
	if (std::find_if(kanji_.begin(), kanji_.end(), [&](kanji_compound kc) {return kanji_str == kc.get_kanji(); }) != kanji_.end()) {
		throw std::logic_error("Duplicate kanji");
	}

	kanji_compound kc{ std::move(kanji_str), std::move(reading), std::move(meaning), ++max_id_, 1, std::chrono::system_clock::now() };
	kanji_.push_back(std::move(kc));
}

void kanji_data::kanji_lib::update_kanji(kanji_compound kanji) {
	auto it = std::find_if(kanji_.begin(), kanji_.end(), [&](kanji_compound kc) {return kanji.get_id() == kc.get_id(); });
	if (!kanji.id_valid() || it == kanji_.end())
	{
		throw std::logic_error("Cannot update kanji - missing value");
	}

	*it = kanji;
}

void kanji_data::kanji_lib::delete_kanji(kanji_compound::kanji_id id)
{
	kanji_.erase(std::remove_if(kanji_.begin(), kanji_.end(), [&](kanji_compound kc) {return kc.get_id() == id; }), kanji_.end());
}

std::vector<kanji_data::kanji_compound> kanji_data::by_kanji(const kanji_lib &lib, wchar_t k)
{
	std::vector<kanji_compound> kanji_k;
	std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(), std::back_inserter(kanji_k), [&](kanji_compound kc) {return kc.get_kanji().find(k) >= 0; });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::by_meaning(const kanji_lib &lib, const std::string &m)
{
	std::vector<kanji_compound> kanji_k;
	std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(), std::back_inserter(kanji_k), [&](kanji_compound kc) {return kc.meaning.find(m) >= 0; });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::by_reading(const kanji_lib &lib, const std::string &r) {
	std::vector<kanji_compound> kanji_k;
	std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(), std::back_inserter(kanji_k), [&](kanji_compound kc) {return kc.meaning.find(r) >= 0; });

	return kanji_k;
}

std::vector<kanji_data::kanji_compound> kanji_data::due_today(const kanji_lib &lib)
{
	std::vector<kanji_compound> kanji_k;
	std::copy_if(lib.get_kanji().begin(), lib.get_kanji().end(), std::back_inserter(kanji_k), [&](kanji_compound kc)
	{
		time t = kc.get_last_rep() + std::chrono::hours(kc.get_level() * 24);
		return t <= std::chrono::system_clock::now();
	});

	return kanji_k;
}

kanji_data::kanji_lib read_lib(std::istream &is)
{
	
}
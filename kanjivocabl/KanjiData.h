#ifndef KANJI_DATA_HPP
#define KANJI_DATA_HPP

#include <string>
#include <vector>

#include <iostream>

#include <chrono>
#include <ctime>

namespace kanji_data
{
	class kanji_compound {
	public:
		kanji_compound(std::wstring k_str, std::string reading, std::string meaning, std::int_least16_t level) :
			kanji_str_{ std::move(k_str) }, reading{ std::move(reading) }, meaning{ std::move(meaning) },
			level_{ level }
		{
			const auto now = std::chrono::system_clock::now();
			last_rep_ = std::chrono::system_clock::to_time_t(now);
		}

		using kanji_id = std::uint_least32_t;


		void repeat(bool succeeded);
		void reset_time();

		std::string reading;
		std::string meaning;

		kanji_id get_id() const { return id_; }
		const std::wstring &get_kanji() const { return kanji_str_; }
		std::time_t get_last_rep() const { return last_rep_; }
		std::int_least16_t get_level() const { return level_; }
	private:
		kanji_id id_;
		std::wstring kanji_str_;

		std::time_t last_rep_;
		std::int_least16_t level_;
	};

	class kanji_lib {
	public:
		explicit kanji_lib(std::vector<kanji_compound> kanji) : kanji_{ std::move(kanji) } {}

		std::vector<kanji_compound> &get_kanji() { return kanji_; }
		void add_kanji(kanji_compound kanji);
		void update_kanji(kanji_compound kanji);
		void delete_kanji(kanji_compound::kanji_id id);
	private:
		std::vector<kanji_compound> kanji_;
	};

	kanji_lib read_kanji(std::istream &is);

	//todo write iterators :)
}

#endif // !KANJI_DATA_HPP

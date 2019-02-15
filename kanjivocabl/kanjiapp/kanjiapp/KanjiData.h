#ifndef KANJI_DATA_HPP
#define KANJI_DATA_HPP

#include <vector>
#include <istream>

#include <chrono>

namespace kanji_data
{
	using time = std::chrono::system_clock::time_point;

	class kanji_compound {
	public:
		using kanji_id = std::uint_least32_t;
		using kanji_level = std::int_least32_t;

        kanji_compound():reading{L""}, meaning{L""}, id_valid_(true),
            id_(0), kanji_str_{ L"" },  level_{ 0 }
        {
            last_rep_ = std::chrono::system_clock::now();
        }

		kanji_compound(std::wstring k_str, std::wstring reading, std::wstring meaning, const kanji_id id, const kanji_level level, time last_rep) :
			reading{ std::move(reading) }, meaning{ std::move(meaning) }, id_valid_(true),
			id_(id), kanji_str_{ std::move(k_str) }, last_rep_{ last_rep }, level_{ level } {}

		kanji_compound(std::wstring k_str, std::wstring reading, std::wstring meaning) :
			reading{ std::move(reading) }, meaning{ std::move(meaning) }, id_valid_(false),
			id_(0), kanji_str_{ std::move(k_str) }, level_{ 1 }
		{
			last_rep_ = std::chrono::system_clock::now();
		}

		void repeat(bool succeeded);
		void reset_time();

		std::wstring reading;
		std::wstring meaning;

		kanji_id get_id() const { return id_; }
		bool id_valid() const { return id_valid_; }

		const std::wstring &get_kanji() const { return kanji_str_; }
		time get_last_rep() const { return last_rep_; }
		kanji_level get_level() const { return level_; }
	private:
		bool id_valid_;

		kanji_id id_;
		std::wstring kanji_str_;

		static const kanji_level max_level = 32;
		time last_rep_;
		kanji_level level_;
	};

	class kanji_lib {
	public:
        kanji_lib(std::vector<kanji_compound> kanji, const kanji_compound::kanji_id id) : max_id_(id), kanji_(std::move(kanji)) {}

        std::vector<kanji_compound> &get_kanji() { return kanji_; }
        const std::vector<kanji_compound> &get_kanji() const { return kanji_; }

        kanji_compound add_kanji(std::wstring kanji_str, std::wstring reading, std::wstring meaning);
		void update_kanji(kanji_compound kanji);
		void delete_kanji(kanji_compound::kanji_id id);
		void reset_time();
	private:
		kanji_compound::kanji_id max_id_;
		std::vector<kanji_compound> kanji_;
	};

    std::vector<kanji_compound> by_kanji(const kanji_lib &lib, const std::wstring &m);
	std::vector<kanji_compound> by_meaning(const kanji_lib &lib, const std::wstring &m);
	std::vector<kanji_compound> by_reading(const kanji_lib &lib, const std::wstring &r);
	std::vector<kanji_compound> due_today(const kanji_lib &lib);

    kanji_lib empty_lib();
	kanji_lib read_lib(std::wistream &is);
	void write_lib(const kanji_lib &lib, std::wostream &wos);
}

#endif // !KANJI_DATA_HPP

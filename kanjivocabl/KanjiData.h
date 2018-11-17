#ifndef KANJI_DATA_HPP
#define KANJI_DATA_HPP

#include <string>
#include <ctime>
#include <chrono>

namespace kanji_data
{
	class kanji_compound {
	public:
		kanji_compound(std::wstring k_str, std::string reading, std::string meaning) :
			kanji_str_{ std::move(k_str) }, reading_{ std::move(reading) }, meaning_{ std::move(meaning) },
			level_{}
		{
			const auto now = std::chrono::system_clock::now();
			last_rep_ = std::chrono::system_clock::to_time_t(now);
		}

	private:
		std::wstring kanji_str_;
		std::string reading_;
		std::string meaning_;

		std::time_t last_rep_;
		std::int_least16_t level_;
	};


}

#endif // !KANJI_DATA_HPP

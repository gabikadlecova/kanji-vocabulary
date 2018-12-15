#include <cstdio>
#include <istream>
#include <fstream>
#include "KanjiData.h"
#include <codecvt>

class a {
public:
	a() :meaning(""), m(L"") {}
	a(std::string a, std::wstring b) : meaning(a), m{ b }{}
	std::string meaning;

private:
	std::wstring m;
};

int main(int argc, char **argv) {
	a k("pr", L"šeøík");
	a b = k;
	a c;
	c = k;
	c = std::move(k);

	kanji_data::kanji_compound k1{ L"a", L"a",L"r" };
	kanji_data::kanji_compound k2 = k1;
	kanji_data::kanji_compound k3 = std::move(k1);

	auto aa = std::chrono::system_clock::now();

	std::wifstream wis{ "testk.txt" };
	wis.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	kanji_data::kanji_lib lib = kanji_data::read_lib(wis);

	wis.close();

	std::wofstream wos{ "textkout.txt" };
	wos.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	kanji_data::write_lib(lib, wos);

	wos.flush();
	wos.close();

	printf("");
}
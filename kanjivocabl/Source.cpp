#include <cstdio>
#include <vector>
#include "KanjiData.h"

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

	kanji_data::kanji_compound k1{ L"a", "a","r" };
	kanji_data::kanji_compound k2 = k1;
	kanji_data::kanji_compound k3 = std::move(k1);

	auto aa = std::chrono::system_clock::now();

	printf("");
}
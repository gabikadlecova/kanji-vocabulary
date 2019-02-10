#include <QApplication>
#include <QVector>

#include <fstream>
#include <codecvt>
#include <iostream>

#include "mainwidget.h"
#include "kanjilistwidget.h"

#include "kanjiapp/KanjiData.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::wifstream wis{ "temp.txt" };
    wis.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    kanji_data::kanji_lib lib = kanji_data::read_lib(wis);

    wis.close();


    MainWidget w(lib);

    w.show();

    return a.exec();
}

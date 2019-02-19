#include <QApplication>

#include "mainwidget.h"
#include "kanjilistwidget.h"

#include "kanjiapp/KanjiData.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string kanjiFileName = "";

    // reads default input file location
    std::ifstream ifs{"kanji.config"};
    if (ifs.good()) {
        ifs >> kanjiFileName;
    }
    ifs.close();

    MainWidget w{kanjiFileName};
    w.show();

    return a.exec();

}

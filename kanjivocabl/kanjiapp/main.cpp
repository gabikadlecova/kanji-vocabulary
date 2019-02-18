#include <QApplication>

#include <iostream>
#include <fstream>

#include "mainwidget.h"
#include "kanjilistwidget.h"

#include "kanjiapp/KanjiData.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string kanjiFileName = "";

    std::ifstream ifs{"kanji.config"};

    if (ifs.good()) {
        ifs >> kanjiFileName;
    }
    ifs.close();

    MainWidget w{kanjiFileName};
    w.show();

    return a.exec();

}

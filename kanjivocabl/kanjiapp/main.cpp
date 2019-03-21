#include <QApplication>

#include "mainwidget.h"
#include "kanjilistwidget.h"

#include "kanjiapp/KanjiData.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget w{ "kanji.config" };
    w.show();

    return a.exec();

}

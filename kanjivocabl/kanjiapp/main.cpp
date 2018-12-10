#include "kanjiwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    KanjiWidget w;
    w.show();

    return a.exec();
}

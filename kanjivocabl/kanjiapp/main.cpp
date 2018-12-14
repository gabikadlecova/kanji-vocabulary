#include <QApplication>

#include "mainwidget.h"
#include "editwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget w;
    w.show();

    return a.exec();
}

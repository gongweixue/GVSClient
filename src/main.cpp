#include "MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    //init the application
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.displayCube();

    return a.exec();
}

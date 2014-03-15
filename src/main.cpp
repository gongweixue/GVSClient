//#include "MainWindow.h"
#include <QtGui/QApplication>
#include "GVSInitializer.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("GVSClient");
    QApplication::setApplicationVersion("v0.1");
    QApplication::setOrganizationName("GVSClient");

    QApplication qtapp(argc, argv);

    GVSInitializer gvsInitializer;
    if (!gvsInitializer.Initialize()) {
        return 1;
    }

    return qtapp.exec();
}
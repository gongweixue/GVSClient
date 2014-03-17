#include "MainWindow.h"
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

    QObject::connect(gvsInitializer.mainWindow->ui.actionQuit,
                     SIGNAL(triggered()),
                     &qtapp,
                     SLOT(quit()));

    return qtapp.exec();
}
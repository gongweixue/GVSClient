#include <QTextCodec>
#include <QtGui/QApplication>
#include "GVSInitializer.h"
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    QApplication::setApplicationName("GVSClient");
    QApplication::setApplicationVersion("v1.0");
    QApplication::setOrganizationName("GVSClient");

    QApplication qtapp(argc, argv);

    GVSInitializer gvsInitializer;
    if (!gvsInitializer.Initialize())
    {
        return 1;
    }

    QObject::connect(gvsInitializer.mainWindow->ui.actionQuit,
                     SIGNAL(triggered()),
                     &qtapp,
                     SLOT(quit()));

    return qtapp.exec();
}
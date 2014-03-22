#include <QBitMap>
#include <QDir>
#include <QSplashScreen>
#include "GVSApplicationCore.h"
#include "GVSInitializer.h"
#include "MainWindow.h"

GVSInitializer::GVSInitializer(void)
{
    this->mainWindow = 0;
    this->gvsApp = 0;
    this->splash = 0;
}


GVSInitializer::~GVSInitializer(void)
{
    delete this->mainWindow;
    this->mainWindow = 0;

    delete this->gvsApp;
    this->gvsApp = 0;

    delete this->splash;
    this->splash = 0;
}

bool GVSInitializer::Initialize()
{
    //create a splash and show before main window created.
    bool show_splash = false;//gvsApp->getSettings()->isShowSplash();
    if (show_splash) {
        QPixmap splashImage(":/Resources/startup.jpg");
        this->splash = new QSplashScreen(splashImage, Qt::WindowStaysOnTopHint);
        this->splash->setMask(
                splashImage.createMaskFromColor(QColor(Qt::transparent)));
        this->splash->show();
    }

    //create the main window.
    this->mainWindow = new MainWindow();

    // Maybe there should be wrote some code to load controls and bars etc.
    /************************************************************************/
    /* Todo: load the widgets, actions, menus, tools and status bar etc.    */
    /************************************************************************/

    //load the configuration after all things about mainWindow done.
    QDir dirConfigFiles(":/GVS/Configuration");//this path should be modified.
    QStringList configFiles = dirConfigFiles.entryList(QDir::Files);
    foreach (QString file, configFiles) {
        this->gvsApp->loadConfiguration(QString(":/GVS/Configuration") + file);
    }

    this->mainWindow->setWindowTitle("GVS v0.1 32-bit");

    // give GUI components time to update before the mainwindow is shown
    QApplication::instance()->processEvents();
    this->mainWindow->show();
    this->mainWindow->welcomeYou();

    QApplication::instance()->processEvents();

    if (show_splash)
    {
        this->splash->finish(this->mainWindow);
    }

    return true;
}
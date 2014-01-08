#pragma once

class MainWindow;
class GVSApplicationCore;
class QSplashScreen;

class GVSInitializer
{
public:
    GVSInitializer(void);
    ~GVSInitializer(void);

    bool Initialize();

private:
    MainWindow* mainWindow;
    GVSApplicationCore* gvsApp;
    QSplashScreen* splash;
};


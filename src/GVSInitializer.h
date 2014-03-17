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

public:
    MainWindow* mainWindow;

private:
    GVSApplicationCore* gvsApp;
    QSplashScreen* splash;
};

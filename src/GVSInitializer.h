#ifndef GVS_INITIALIZER_H
#define GVS_INITIALIZER_H

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

#endif

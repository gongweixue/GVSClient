#ifndef GVS_APLLICATIONS_CORE_H
#define GVS_APLLICATIONS_CORE_H

class GVSSettings;
class QString;

class GVSApplicationCore {
public:
    GVSApplicationCore(void);
    ~GVSApplicationCore(void);

    GVSSettings* getSettings();

    // load the xml or json or txt files to config the env of gvs.
    void loadConfiguration(const QString& fileName);
protected:
    GVSSettings* settings;
};

#endif

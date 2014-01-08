#include "GVSApplicationCore.h"
#include "GVSSettings.h"


GVSApplicationCore::GVSApplicationCore(void)
{
    this->settings = new GVSSettings();
}


GVSApplicationCore::~GVSApplicationCore(void)
{
    delete this->settings;
    this->settings = 0;
}

GVSSettings* GVSApplicationCore::getSettings()
{
    return this->settings;
}

void GVSApplicationCore::loadConfiguration( const QString& fileName )
{
    //Todo:
    //Parse the config file indicated the file name, load the configuration
    // and set the env of gvs.
}

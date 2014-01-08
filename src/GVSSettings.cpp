#include "GVSSettings.h"


GVSSettings::GVSSettings(void)
{
    this->showSplash = true;
}


GVSSettings::~GVSSettings(void)
{
}

bool GVSSettings::isShowSplash()
{
    return this->showSplash;
}

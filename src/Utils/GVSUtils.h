#ifndef GVS_UTILS_H
#define GVS_UTILS_H

#include <string>
#include <QDomDocument>
#include <QFile>

template<typename T>
bool DeleteVTKPointer(T& pointer)
{
    if (pointer!=0)
    {
        pointer->Delete();
        pointer = 0;
        return true;
    }
    return false;
};

class GVSUtils
{
};


#endif
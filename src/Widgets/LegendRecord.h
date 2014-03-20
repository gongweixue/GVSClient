#ifndef LEGENDRECORD_H
#define LEGENDRECORD_H

#include <string>
#include <QColor>

using namespace std;

class LegendRecord{
public:
    int id;
    string name;
    QColor rgb;
    string description;
    LegendRecord(int lrID, string lrName, QColor lrRGB, string lrDesc);
    LegendRecord();
};



#endif

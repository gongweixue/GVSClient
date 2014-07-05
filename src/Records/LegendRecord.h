#ifndef LEGEND_RECORD_H
#define LEGEND_RECORD_H

#include <string>
#include <QColor>

using namespace std;

class LegendRecord
{
public:
    string name;
    QColor rgb;
    string description;
    LegendRecord(string lrName, QColor lrRGB, string lrDesc);

    LegendRecord();
};

#endif

#include "LegendRecord.h"

LegendRecord::LegendRecord(int lrID, string lrName, QColor lrRGB, string lrDesc) 
    :id(lrID), name(lrName), rgb(lrRGB), description(lrDesc)
{
}

LegendRecord::LegendRecord()
{

}


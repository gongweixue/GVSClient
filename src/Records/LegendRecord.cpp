#include "LegendRecord.h"

LegendRecord::LegendRecord(const string& lrName, const QColor& lrRGB, const string& lrDesc)
    : name(lrName), rgb(lrRGB), description(lrDesc) {
}

LegendRecord::LegendRecord() {

}


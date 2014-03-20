#ifndef LEGENDITEM_H
#define LEGENDITEM_H

#include <QWidget>
#include "ui_LegendItem.h"
#include "Widgets/LegendRecord.h"

class LegendItem : public QWidget
{
    Q_OBJECT

public:
    LegendItem(LegendRecord rcrd, QWidget *parent = 0);
    ~LegendItem();

private:
    Ui::LegendItem ui;
    LegendRecord record;
};

#endif // LEGENDITEM_H

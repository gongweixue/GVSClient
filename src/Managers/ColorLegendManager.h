#ifndef COLORLEGENDMANAGER_H
#define COLORLEGENDMANAGER_H

#include <QObject>
#include <QDockWidget>
#include <string>
#include <vector>
#include <QColor>
#include <QVBoxLayout>

#include "Widgets/LegendItem.h"
#include "Widgets/LegendRecord.h"

using namespace std;

class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    
    ColorLegendManager(QDockWidget* pLegend, string path, QObject* parent = 0);
    ~ColorLegendManager();

public slots:
    void initOrUpdateLegend();//should be called after edition on the legend dock.

private:
    ColorLegendManager(QObject* parent);
    void parseLegendNames();
    void fillLegendDock();
    void genericItems();
    void clearVecOfItems();
private:
    QDockWidget* m_pLegend;
    string m_pathNameOfProject;
    vector<LegendRecord> vecOfLegendRecord;
    vector<LegendItem*> vecOfLegendItem;
    QVBoxLayout* pLayout;
};

#endif // COLORLEGENDMANAGER_H

#ifndef COLORLEGENDMANAGER_H
#define COLORLEGENDMANAGER_H

#include <QObject>
#include <QDockWidget>
#include <string>
#include <vector>
#include <QColor>
#include <QVBoxLayout>
#include <QListWidgetItem>

#include "Widgets/LegendRecord.h"

using namespace std;

class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    
    ColorLegendManager(QListWidget* pListWidget, string path, QObject* parent = 0);
    ~ColorLegendManager();

public slots:
    //should be called after edition on the legend dock.
    void initOrUpdateLegend();

private:
    ColorLegendManager(QObject* parent);
    void parseLegendNames();
    void fillLegendDock();
    void genericItems();
private:
    QListWidget* m_pListWidget;
    string m_pathNameOfProject;
    vector<LegendRecord> vecOfLegendRecord;
    vector<QListWidgetItem> vecOfLegendItem;
};

#endif // COLORLEGENDMANAGER_H

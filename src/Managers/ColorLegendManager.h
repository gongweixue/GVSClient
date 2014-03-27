#ifndef COLORLEGENDMANAGER_H
#define COLORLEGENDMANAGER_H

#include <QObject>
#include <QDockWidget>
#include <string>
#include <vector>
#include <QColor>
#include <QVBoxLayout>
#include <QListWidgetItem>

#include "Records/LegendRecord.h"

using namespace std;

class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    ColorLegendManager(QListWidget* pListWidget, string path, QObject* parent = 0);
    ~ColorLegendManager();
    string getFilePath() {return projectFilePath;}
    bool insertItemToFile(string name, QColor rgb, string description);

public slots:
    //should be called after edition on the legend dock.
    void initOrUpdateLegend(string gvpFullFileName);
    vector<LegendRecord>* getVecPtrOfRecord() {return &vecOfLegendRecord;}

private:
    ColorLegendManager(QObject* parent);
    void parseLegendNames(string gvpFullFileName);
    void fillLegendDock();
    void genericItems();
private:
    QListWidget* m_pListWidget;
    vector<LegendRecord> vecOfLegendRecord;
    vector<QListWidgetItem> vecOfLegendItem;
    string projectFilePath;
};

#endif // COLORLEGENDMANAGER_H

#ifndef COLOR_LEGEND_MANAGER_H
#define COLOR_LEGEND_MANAGER_H

#include <string>
#include <vector>
#include <QColor>
#include <QDockWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QVBoxLayout>
#include "Records/LegendRecord.h"

using namespace std;

class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    ColorLegendManager(QListWidget* pListWidget, string path, QObject* parent = 0);
    ~ColorLegendManager();
    string getFilePath() {return projectFilePath;}
    bool insertItemToFile(const string& name, const QColor& rgb, const string& description);
    bool editItemInFile(const string& name, const QColor& rgb, const string& description);
    bool delItemFromFile(const char * name);

public slots:
    //should be called after edit on the legend dock.
    void initOrUpdateLegend(const string& gvpFullFileName);
    vector<LegendRecord>* getVecPtrOfRecord() {return &vecOfLegendRecord;}

private:
    ColorLegendManager(QObject* parent);
    void parseLegendNames(const string& gvpFullFileName);
    void fillLegendDock();
    void genericItems();

private:
    QListWidget* m_pListWidget;
    vector<LegendRecord> vecOfLegendRecord;
    vector<QListWidgetItem> vecOfLegendItem;
    string projectFilePath;
};

#endif // COLOR_LEGEND_MANAGER_H

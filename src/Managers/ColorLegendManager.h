#ifndef COLORLEGENDMANAGER_H
#define COLORLEGENDMANAGER_H

#include <QObject>
#include <QDockWidget>
#include <string>
#include <vector>
#include <QColor>

using namespace std;

class LegendItem;

struct LegendRecord{
    string name;
    QColor rgb;
    string description;
};

class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    ColorLegendManager(QObject* parent);
    ColorLegendManager(QDockWidget* pLegend, string path, QObject* parent = 0);
    ~ColorLegendManager();

    vector<LegendRecord>* getVecOfLegend() {return &vecOfLegendRecord;}

public slots:
    void initOrUpdateLegend();//should be called when edit the legend items.

private:
    void parseProjectLegend();
    void fillLegendDock();

private:
    QDockWidget* m_pLegend;
    string m_pathNameOfProject;
    vector<LegendRecord> vecOfLegendRecord;
    vector<LegendItem> vecOfLegendItem;
};

#endif // COLORLEGENDMANAGER_H

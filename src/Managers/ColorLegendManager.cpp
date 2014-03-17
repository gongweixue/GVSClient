#include "ColorLegendManager.h"

ColorLegendManager::ColorLegendManager(QObject *parent)
    : QObject(parent)
{
}

ColorLegendManager::ColorLegendManager(QDockWidget* pLegend, string path, QObject* parent)
    : QObject(parent)
{
    m_pLegend = pLegend;
    m_pathNameOfProject = path;
    initOrUpdateLegend();
}

ColorLegendManager::~ColorLegendManager()
{
}

void ColorLegendManager::initOrUpdateLegend()
{
    parseProjectLegend();
    fillLegendDock();
}

void ColorLegendManager::parseProjectLegend()
{
    //TODO:parse the xml project file, put the record into the vector of legRecord.
    //erase the table of record first
    //then parse
}

void ColorLegendManager::fillLegendDock()
{
    //TODO:iterate the vector of the legend record,
    //and make item widget by three data members.
    
    //Erase the table of items
    //then make the widget of items by the table of record.
    //and add them onto the dock widget of main window.
}

#include "ColorLegendManager.h"

ColorLegendManager::ColorLegendManager(QObject *parent)
    : QObject(parent)
{
    throw std::exception("ColorLegendManage's Default constructor is not implement.");
}

ColorLegendManager::ColorLegendManager(QDockWidget* pLegend, string path, QObject* parent)
    : QObject(parent)
{
    m_pLegend = pLegend;
    m_pathNameOfProject = path;
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();
    pLayout = new QVBoxLayout(m_pLegend);
    m_pLegend->setLayout(pLayout);
    initOrUpdateLegend();
}

ColorLegendManager::~ColorLegendManager()
{
    clearVecOfItems();
    if (pLayout)
    {
        delete pLayout;
    }

}

void ColorLegendManager::initOrUpdateLegend()
{
    clearVecOfItems();
    vecOfLegendRecord.clear();
    parseLegendNames();
    genericItems();
    fillLegendDock();
}

void ColorLegendManager::parseLegendNames()
{
    //TODO:parse the xml project file, put the record into the vector of legRecord.

    //Locate on the legend parent element.
    //Find first legend item.
    while (false/*first item exist*/) {
            //read the record's id, name, rgb, description.
            //if there is one not exist, then find next item and continue next loop.
            //else, make up a record with id, name, rgb and description.
            //push back into the vector of record
    }


    ///////////mock some record ///////////////////////
    LegendRecord lr1(1, "ABC", QColor(30, 80, 90), "description 1");
    LegendRecord lr2(2, "ABC", QColor(130, 100, 70), "description 2");
    LegendRecord lr3(3, "ABC", QColor(230, 180, 190), "description 3");
    vecOfLegendRecord.push_back(lr1);
    vecOfLegendRecord.push_back(lr2);
    vecOfLegendRecord.push_back(lr3);
}

void ColorLegendManager::genericItems()
{
    //TODO:iterate the vector of the legend record
    vector<LegendRecord>::const_iterator iter;
    for (iter = vecOfLegendRecord.begin(); iter != vecOfLegendRecord.end(); iter++)
    {
        LegendItem* pItem = new LegendItem(*iter);
        vecOfLegendItem.push_back(pItem);
    }
}

void ColorLegendManager::fillLegendDock()
{
    //TODO: add the item widgets onto the dock widget of main window.
    if (pLayout)
    {
        vector<LegendItem*>::iterator iter = vecOfLegendItem.begin();
        for (iter = vecOfLegendItem.begin(); iter != vecOfLegendItem.end(); iter++)
        {
            pLayout->addWidget(*iter);
        }
    }
}

void ColorLegendManager::clearVecOfItems()
{
    if (!vecOfLegendItem.empty())
    {
        vector<LegendItem*>::const_iterator iter;
        for (iter = vecOfLegendItem.begin(); iter != vecOfLegendItem.end(); iter++)
        {
            delete *iter;
        }
    }
    vecOfLegendItem.clear();
}


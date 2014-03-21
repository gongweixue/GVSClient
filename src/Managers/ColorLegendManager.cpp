#include "ColorLegendManager.h"
#include <sstream>

ColorLegendManager::ColorLegendManager(QObject *parent)
    : QObject(parent)
{
    throw std::exception("ColorLegendManage's Default constructor is not implement.");
}

ColorLegendManager::ColorLegendManager(QListWidget* pListWidget, string path, QObject* parent)
    : QObject(parent)
{
    m_pListWidget = pListWidget;
    m_pathNameOfProject = path;
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();

    connect(m_pListWidget, SIGNAL(itemSelectionChanged()),
            m_pListWidget, SLOT(clearSelection()));

    initOrUpdateLegend();
}

ColorLegendManager::~ColorLegendManager()
{
}

void ColorLegendManager::initOrUpdateLegend()
{
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();

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
    for (int i = 0; i < 50; ++i)
    {
        string str = "num ";
        std::ostringstream oss;
        oss << i << " ";
        str += oss.str();
        LegendRecord lr(i, str, QColor((i * 10)%255, (i*20)%255, (i*30)%255), str + "description");
        vecOfLegendRecord.push_back(lr);
    }
}

void ColorLegendManager::genericItems()
{
    if (vecOfLegendRecord.empty())
    {
        return;
    }

    vector<LegendRecord>::const_iterator iter;
    for (iter = vecOfLegendRecord.begin(); iter != vecOfLegendRecord.end(); iter++)
    {
        QString itemName(tr("\n"));
        itemName.append(tr(iter->name.c_str()));
        itemName.append(tr("\n"));
        QListWidgetItem item(itemName);

        item.setBackgroundColor(iter->rgb);
        QColor textColor(255 - iter->rgb.red(), 
                       255 - iter->rgb.green(), 
                       255 - iter->rgb.blue());
        item.setTextColor(textColor);
        item.setTextAlignment(Qt::AlignHCenter);

        //TODO set the font size, 

        item.setToolTip(tr(iter->description.c_str()));
        vecOfLegendItem.push_back(item);
    }
}

void ColorLegendManager::fillLegendDock()
{
    if (!vecOfLegendItem.empty())
    {
        vector<QListWidgetItem>::iterator iter = vecOfLegendItem.begin();
        for (iter = vecOfLegendItem.begin(); iter != vecOfLegendItem.end(); iter++)
        {
            m_pListWidget->addItem(&(*iter));
        }
    }
}



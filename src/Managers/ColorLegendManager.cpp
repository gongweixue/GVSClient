#include "ColorLegendManager.h"
#include <sstream>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>

ColorLegendManager::ColorLegendManager(QObject *parent)
    : QObject(parent)
{
    throw std::exception("ColorLegendManage's Default constructor is not implement.");
}

ColorLegendManager::ColorLegendManager(QListWidget* pListWidget, string path, QObject* parent)
    : QObject(parent)
{
    m_pListWidget = pListWidget;
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();

    connect(m_pListWidget, SIGNAL(itemSelectionChanged()),
            m_pListWidget, SLOT(clearSelection()));

    initOrUpdateLegend(path);
}

ColorLegendManager::~ColorLegendManager()
{
}

void ColorLegendManager::initOrUpdateLegend(string pathOfProjectFile)
{
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();

    parseLegendNames(pathOfProjectFile);

    genericItems();

    fillLegendDock();
}

void ColorLegendManager::parseLegendNames(string pathOfProjectFile)
{
    if(pathOfProjectFile.empty())
        return;  

    QFile file(pathOfProjectFile.c_str());
    if( !file.open(QFile::ReadOnly | QFile::Text) ) {
        printf("open file '%s' failed, error: %s !\n", 
            pathOfProjectFile.c_str(),
            file.errorString().toStdString().c_str());
        return;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    if( !domDoc.setContent(&file, false, &strError, &errLine, &errCol) ) {
        printf("parse file failed at line %d column %d, %s !\n",
            errLine, errCol, strError );
        file.close();
        return;
    }

    if( domDoc.isNull() ) {
        printf( "document is null !\n" );
        file.close();
        return;
    }

    //get the root of file
    QDomElement root = domDoc.documentElement();
    QDomNodeList list = root.childNodes();
    int countNodeOfProject = list.count();
    for (int i = 0; i < countNodeOfProject; ++i)
    {
        QDomNode node = list.item(i);
        QDomElement childOfProject = node.toElement();
        QString tagChild = childOfProject.tagName();
        //printf("tag of project %s.\n", tagChild.toStdString().c_str());
        if (tagChild.compare("ColorLegendItems"))
        {
            continue;
        }
        else 
        {
            QDomNodeList nodeListOfItems = childOfProject.childNodes();
            int countItems = nodeListOfItems.count();
            for (int j = 0; j < countItems; ++j)
            {
                LegendRecord rcrd;
                rcrd.id = j + 1;
                //get name of item
                QDomNode itemNode = nodeListOfItems.item(j);
                QDomElement elementLegendItem = itemNode.toElement();
                QDomElement itemName = elementLegendItem.firstChildElement();
                rcrd.name = itemName.text().toStdString();
                //get rgb of item
                QDomElement red = itemName.nextSiblingElement();
                rcrd.rgb.setRed(red.text().toInt() % 255);
                QDomElement green = red.nextSiblingElement();
                rcrd.rgb.setGreen(green.text().toInt() % 255);
                QDomElement blue = green.nextSiblingElement();
                rcrd.rgb.setBlue(blue.text().toInt() % 255);
                //get description of item
                QDomElement desc = blue.nextSiblingElement();
                rcrd.description = desc.text().toStdString();

                vecOfLegendRecord.push_back(rcrd);
            }
        }
        break;
    }

    file.close();


//     for (int i = 0; i < 50; ++i)
//     {
//         string str = "num ";
//         std::ostringstream oss;
//         oss << i << " ";
//         str += oss.str();
//         LegendRecord lr(i, str, QColor((i * 10)%255, (i*20)%255, (i*30)%255), str + "description");
//         vecOfLegendRecord.push_back(lr);
//     }
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



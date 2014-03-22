#include "ColorLegendManager.h"
#include <sstream>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>

ColorLegendManager::ColorLegendManager(QObject *parent)
    : QObject(parent)
{
    throw std::exception("ColorLegendManage's Default constructor is not implement.");
}

ColorLegendManager::ColorLegendManager(QListWidget* pListWidget,
    string path, QObject* parent)
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

void ColorLegendManager::initOrUpdateLegend(string gvpFullFileName)
{
    vecOfLegendRecord.clear();
    vecOfLegendItem.clear();

    parseLegendNames(gvpFullFileName);

    genericItems();

    fillLegendDock();
}

void ColorLegendManager::parseLegendNames(string gvpFullFileName)
{
    if(gvpFullFileName.empty())
        return;  

    QFile file(gvpFullFileName.c_str());
    if( !file.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::information(NULL, tr("加载图例失败"), file.errorString());
        return;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    if( !domDoc.setContent(&file, false, &strError, &errLine, &errCol) ) {
        QMessageBox::information(NULL, tr("加载图例失败"), tr("文件格式不正确"));
        file.close();
        return;
    }

    if( domDoc.isNull() ) {
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
        if (0 != tagChild.compare("ColorLegendItems"))
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
            break;
        }
    }

    file.close();
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



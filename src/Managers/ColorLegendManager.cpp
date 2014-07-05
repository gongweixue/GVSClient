#include <sstream>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "ColorLegendManager.h"

ColorLegendManager::ColorLegendManager(QObject* parent)
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

    projectFilePath = gvpFullFileName;
    parseLegendNames(gvpFullFileName);

    genericItems();

    fillLegendDock();
}

void ColorLegendManager::parseLegendNames(string gvpFullFileName)
{
    if(gvpFullFileName.empty())
        return;

    QFile file(gvpFullFileName.c_str());
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        QMessageBox::information(NULL, tr("加载图例失败"), file.errorString());
        return;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    if( !domDoc.setContent(&file, false, &strError, &errLine, &errCol) )
    {
        QMessageBox::information(NULL, tr("加载图例失败"), tr("文件格式不正确"));
        file.close();
        return;
    }

    if( domDoc.isNull() )
    {
        file.close();
        return;
    }

    //get the root of file
    QDomElement root = domDoc.documentElement();
    QDomNodeList list = domDoc.elementsByTagName("ColorLegendItem");
    int countNodeOfProject = list.count();
    for (int i = 0; i < countNodeOfProject; ++i)
    {
        QDomNode node = list.item(i);
        QDomElement childOfProject = node.toElement();
        QString tagChild = childOfProject.tagName();

        //Is this nesessary?
        if (0 != tagChild.compare("ColorLegendItem"))
        {
            continue;
        }
        else
        {
            LegendRecord rcrd;
            rcrd.name = childOfProject.attribute("name").toStdString();
            rcrd.rgb.setRed(childOfProject.attribute("r").toInt());
            rcrd.rgb.setGreen(childOfProject.attribute("g").toInt());
            rcrd.rgb.setBlue(childOfProject.attribute("b").toInt());
            rcrd.description = childOfProject.text().toStdString();

            vecOfLegendRecord.push_back(rcrd);
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
        QColor textColor((128 + iter->rgb.red()) % 256,
                         (128 + iter->rgb.green()) % 256,
                         (128 + iter->rgb.blue()) % 256);
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

bool ColorLegendManager::insertItemToFile(string name, QColor rgb, string description)
{
    if(projectFilePath.empty())
        return false;

    QFile file(projectFilePath.c_str());
    if( !file.open(QIODevice::ReadOnly) ) {
        QMessageBox::information(NULL, tr("插入图例失败"), file.errorString());
        return false;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    bool isSetContentOk = domDoc.setContent(&file);
    file.close();

    if(!isSetContentOk) {
        QMessageBox::information(NULL, tr("插入图例失败"), tr("文件格式不正确"));
        return false;
    }

    if(domDoc.isNull()) {
        return false;
    }

    //get the root of file
    QDomElement root = domDoc.documentElement();
    QDomNodeList list = domDoc.elementsByTagName("ColorLegendItem");

    //first, we should sure that there is no node
    //which has the same name with the node to be insert.
    int countNodeOfProject = list.count();
    for (int i = 0; i < countNodeOfProject; ++i)
    {
        QDomNode node = list.item(i);
        QDomElement childOfProject = node.toElement();
        QString tagChild = childOfProject.tagName();
        if (0 != tagChild.compare("ColorLegendItem"))
        {
            continue;
        }
        else
        {
            //if has the node with same name
            if ( 0 == childOfProject.attribute("name").compare(name.c_str()) )
            {
                QString tip = tr("已经存在名为'");
                tip.append(name.c_str());
                tip.append("'的图例！");

                QMessageBox::information(NULL, tr("提示"), tip);
                return false;
            }
        }
    }

    //start inserting.
    QDomElement item = domDoc.createElement("ColorLegendItem");
    item.setAttribute("name", name.c_str());
    item.setAttribute("r", rgb.red());
    item.setAttribute("g", rgb.green());
    item.setAttribute("b", rgb.blue());

    QDomText descText = domDoc.createTextNode(description.c_str());
    item.appendChild(descText);

    //insert the item.
    if (root.appendChild(item).isNull())
    {
        return false;
    }
    //write back to gvp
    QFile fileModify(projectFilePath.c_str());
    if (!fileModify.open(QFile::WriteOnly | QFile::Text)){
        return false;
    }
    QTextStream out(&fileModify);
    domDoc.save(out, 4);
    fileModify.close();
    return true;
}

bool ColorLegendManager::editItemInFile(string name, QColor rgb, string description)
{
    if(projectFilePath.empty())
        return false;

    QFile file(projectFilePath.c_str());
    if( !file.open(QIODevice::ReadOnly) ) {
        QMessageBox::information(NULL, tr("编辑图例失败"), file.errorString());
        return false;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    bool isSetContentOk = domDoc.setContent(&file);
    file.close();

    if(!isSetContentOk) {
        QMessageBox::information(NULL, tr("编辑图例失败"), tr("文件格式不正确"));
        return false;
    }

    if(domDoc.isNull()) {
        return false;
    }

    //get the root of file
    QDomElement root = domDoc.documentElement();
    QDomNodeList list = domDoc.elementsByTagName("ColorLegendItem");

    //Find the item to be edit
    int countNodeOfProject = list.count();
    for (int i = 0; i < countNodeOfProject; ++i)
    {
        QDomNode node = list.item(i);
        QDomElement childOfProject = node.toElement();
        QString tagChild = childOfProject.tagName();
        if (0 != tagChild.compare("ColorLegendItem"))
        {
            continue;
        }
        else
        {
            //if has the nodeToDel with same name
            if ( 0 == childOfProject.attribute("name").compare(name.c_str()) )
            {
                //Update the info of this legend.
                childOfProject.setAttribute("r", rgb.red());
                childOfProject.setAttribute("g", rgb.green());
                childOfProject.setAttribute("b", rgb.blue());

                //update desc
                childOfProject.firstChild().setNodeValue(description.c_str());

                //write back to gvp
                QFile fileModify(projectFilePath.c_str());
                if (!fileModify.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::information(NULL,
                                             tr("编辑图例失败"),
                                             fileModify.errorString());
                    return false;
                }
                QTextStream out(&fileModify);
                domDoc.save(out, 4);
                fileModify.close();

                return true;
            }
        }
    }

    QMessageBox::information(NULL, tr("错误"),tr("没有找到指定的图例。"));
    return false;
}

bool ColorLegendManager::delItemFromFile(const char * name)
{
    if(projectFilePath.empty())
        return false;

    QFile file(projectFilePath.c_str());
    if( !file.open(QIODevice::ReadOnly) ) {
        QMessageBox::information(NULL, tr("删除图例失败"), file.errorString());
        return false;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    bool isSetContentOk = domDoc.setContent(&file);
    file.close();

    if(!isSetContentOk) {
        QMessageBox::information(NULL, tr("删除图例失败"), tr("文件格式不正确"));
        return false;
    }

    if(domDoc.isNull()) {
        return false;
    }

    //get the root of file
    QDomElement root = domDoc.documentElement();
    QDomNodeList list = domDoc.elementsByTagName("ColorLegendItem");

    //Find the item to be delete
    int countNodeOfProject = list.count();
    for (int i = 0; i < countNodeOfProject; ++i)
    {
        QDomNode nodeToDel = list.item(i);
        QString tagChild = nodeToDel.toElement().tagName();
        if (0 != tagChild.compare("ColorLegendItem"))
        {
            continue;
        }
        else
        {
            //if has the nodeToDel with same name
            if ( 0 == nodeToDel.toElement().attribute("name").compare(name) )
            {
                //delete the node
                root.removeChild(list.at(i));

                //write back to gvp
                QFile fileModify(projectFilePath.c_str());
                if (!fileModify.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::information(NULL,
                        tr("删除图例失败"),
                        fileModify.errorString());
                    return false;
                }

                QTextStream out(&fileModify);
                domDoc.save(out, 4);
                fileModify.close();

                return true;
            }
        }
    }

    QMessageBox::information(NULL, tr("错误"),tr("没有找到指定的图例。"));
    return false;
}


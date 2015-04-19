#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QMessageBox>
#include "GVSDoc.h"

GVSDoc::GVSDoc(MainWindow* mainWindow, QObject* parent)
    : QObject(parent)
{
    m_pMainWindow = m_pMainWindow;
    m_gvpFullFileName = "";
}

GVSDoc::~GVSDoc()
{
}

bool GVSDoc::OnOpenProject()
{
    string path = getProjectPathByDlg();
    if (path.empty() || !(path.compare(m_gvpFullFileName)))
    {
        return false;
    }
    //Store path to be used.
    m_gvpFullFileName = path;

    m_objManager.ClearObjTree();
    return LoadProject(m_gvpFullFileName);
}

bool GVSDoc::OnCloseDocument()
{
    m_objManager.ClearObjTree();
    return true;
}

bool GVSDoc::LoadProject(const std::string& gvpFullFileName)
{
    return parseProjectByFileName(gvpFullFileName);
}

bool GVSDoc::parseProjectByFileName(const std::string& gvpFullPath)
{
    if (LoadFavTree(gvpFullPath) == false)
    {
        return false;
    }
    if (LoadObjTree(gvpFullPath) == false)
    {
        return false;
    }

    return true;
}

bool GVSDoc::parseAndLoadModel(const std::string& gvmFullPath)
{
    if (gvmFullPath.empty())
        return false;

    QFileInfo gvmFileInfo(gvmFullPath.c_str());
    QString currentDir(gvmFileInfo.absolutePath() + "/");
    QString modelDataDir(currentDir + gvmFileInfo.baseName() + "/");

    QFile gvmFile(gvmFullPath.c_str());
    if(!gvmFile.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    //To be used
    QString strError;
    int errLine = 0, errCol = 0;

    QDomDocument domDocOfGVM;
    if( !domDocOfGVM.setContent(&gvmFile, false, &strError, &errLine, &errCol) )
    {
        gvmFile.close();
        return false;
    }

    if( domDocOfGVM.isNull() )
    {
        gvmFile.close();
        return false;
    }

    vector<Model>::iterator iter_model = m_objManager.treeOfGeoObjs.begin();
    for ( ; iter_model != m_objManager.treeOfGeoObjs.end(); iter_model++)
    {
        if (0 == iter_model->getModelName().compare(gvmFileInfo.baseName()))
        {
            break;
        }
    }

    QDomElement root = domDocOfGVM.documentElement();
    QDomNodeList listOfRootNode = root.childNodes();
    int numOfRootNode = listOfRootNode.count();
    for (int i = 0; i < numOfRootNode; ++i)
    {
        QDomElement childElement = listOfRootNode.item(i).toElement();
        QString typeTag = childElement.tagName();

        //if not object tag
        if (0 != typeTag.compare("Point") &&
            0 != typeTag.compare("Line") &&
            0 != typeTag.compare("Surface"))
        {
            continue;
        }

        QString objFileName = childElement.text();

        QString attrVis = childElement.attribute("visible");
        bool vis;
        if (1 == attrVis.toInt())
            vis = true;
        else if (0 == attrVis.toInt())
            vis =false;
        else
            continue;

        QString textureFileName = childElement.attribute("texture");
        if ("" != textureFileName.toStdString())
        {
            textureFileName = modelDataDir + textureFileName;
        }

        //reader
        vtkDataSetReader* reader = vtkDataSetReader::New();
        reader->SetFileName((modelDataDir + objFileName).toStdString().c_str());

        //type
        int objFileTy;
        if (0 == typeTag.compare("Point"))
        {
            objFileTy = GEO_OBJECT_TYPE_POINT;
        }
        else if (0 == typeTag.compare("Line"))
        {
            objFileTy = GEO_OBJECT_TYPE_LINE;
        }
        else if (0 == typeTag.compare("Surface"))
        {
            objFileTy = GEO_OBJECT_TYPE_SURFACE;
        }

        GeoObject geoObj(objFileName, objFileTy, reader, vis, textureFileName);
        //insert GeoObject into tree
        iter_model->getVecOfGeoObjs()->push_back(geoObj);

    }
    gvmFile.close();

    return true;
}

std::string GVSDoc::getProjectPathByDlg()
{
    QString fileName = QFileDialog::getOpenFileName(
                               0,
                               tr("打开项目文件"),
                               tr("D:\\workspace\\gvsclient\\Example-cw06"),
                               tr("所有文件(*.*);;GVP项目文件(*.gvp)"),
                               &tr("GVP项目文件(*.gvp)"));

    return fileName.toStdString();
}

bool GVSDoc::LoadFavTree(const std::string& filePath)
{
    QFileInfo fileInfo(filePath.c_str());
    QString currentDir(fileInfo.absolutePath() + "/");
    QString prjDataDir(currentDir + fileInfo.baseName() + "/");

    QString favPath = prjDataDir + fileInfo.baseName() + ".fav";
    QFile favFile(favPath);
    if(!favFile.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    //To be used
    QString strError;
    int errLine = 0, errCol = 0;

    QDomDocument domDocOfFav;
    if( !domDocOfFav.setContent(&favFile, false, &strError, &errLine, &errCol) )
    {
        favFile.close();
        return false;
    }

    favFile.close();

    if( domDocOfFav.isNull() )
    {
        return false;
    }

    QDomElement root = domDocOfFav.documentElement();
    QDomNodeList listOfGroupNode = root.childNodes();
    int numOfGroup = listOfGroupNode.count();
    for (int i = 0; i < numOfGroup; ++i)
    {
        QDomElement groupElement = listOfGroupNode.item(i).toElement();
        if (0 != groupElement.tagName().compare("Group"))
        {
            continue;
        }

        m_objManager.getFavTree()->push_back(FavGroup(groupElement.attribute("name").toStdString()));
        vector<FavGroup>::reverse_iterator fldrIter = m_objManager.getFavTree()->rbegin();

        QDomNodeList listOfFavItem = groupElement.childNodes();
        int numOfItem = listOfFavItem.count();
        for (int j = 0; j < numOfItem; ++j)
        {
            QDomElement favItemElement = listOfFavItem.item(j).toElement();
            if (0 != favItemElement.tagName().compare("FavItem"))
            {
                continue;
            }
            QString itemName = favItemElement.attribute("name");
            QString itemModel = favItemElement.attribute("model");
            QString itemObj = favItemElement.attribute("obj");
            fldrIter->getVecOfItems()->push_back(FavItem(itemName, itemModel, itemObj));
        }
    }

    return true;
}

bool GVSDoc::LoadObjTree(const std::string& filePath)
{
    QFileInfo fileInfo(filePath.c_str());
    QString currentDir(fileInfo.absolutePath() + "/");
    QString prjDataDir(currentDir + fileInfo.baseName() + "/");

    if (filePath.empty())
        return false;

    QFile gvpFile(filePath.c_str());
    QDomDocument domDocOfGVP;

    if( !gvpFile.open(QFile::ReadOnly | QFile::Text) )
    {
        QMessageBox::information(NULL, tr("打开项目失败"), gvpFile.errorString());
        return false;
    }

    QString strError;
    int errLine = 0, errCol = 0;
    if(!domDocOfGVP.setContent(&gvpFile, false, &strError, &errLine, &errCol))
    {
        QMessageBox::information(NULL, tr("打开项目失败"), tr("非法项目文件"));
        gvpFile.close();
        return false;
    }

    if(domDocOfGVP.isNull())
    {
        QMessageBox::information(NULL, tr("打开项目失败"), tr("项目为空"));
        gvpFile.close();
        return false;
    }

    //get the root of file
    QDomElement root = domDocOfGVP.documentElement();
    QDomNodeList listOfRootNode = domDocOfGVP.elementsByTagName("Model");
    int numOfRootNode = listOfRootNode.count();
    for (int i = 0; i < numOfRootNode; ++i)
    {
        QDomElement childElement = listOfRootNode.item(i).toElement();
        QString elementTagName = childElement.tagName();
        if (0 != elementTagName.compare("Model"))// NOT <Model>
        {
            continue;
        }
        else
        {
            Model model(childElement.text().toStdString().c_str());
            m_objManager.treeOfGeoObjs.push_back(model);
        }
    }
    gvpFile.close();

    for (int i = 0; i < (int)(m_objManager.treeOfGeoObjs.size()); ++i)
    {
        QString modelName = m_objManager.treeOfGeoObjs[i].getModelName();
        QString gvmFullPath(prjDataDir + modelName + ".gvm");
        if (!parseAndLoadModel(gvmFullPath.toStdString()))
        {
            QMessageBox::information(NULL, tr("警告"), tr("模型加载错误：") + modelName);
            continue;
        }
    }

    //Now we just put the file names of the object in disk into the tree.
    //Then we need to load all of these object into readers(by update).
    m_objManager.LoadDataForReadersInTree();

    return true;
}


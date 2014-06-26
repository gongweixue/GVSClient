#include "GVSDoc.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QMessageBox>

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

void GVSDoc::OnCloseDocument()
{
    m_objManager.ClearObjTree();
}

bool GVSDoc::LoadProject(std::string gvpFullFileName)
{
    return parseProjectByFileName(gvpFullFileName);
}

bool GVSDoc::parseProjectByFileName(std::string gvpFullPath)
{
    QFileInfo gvpFileInfo(gvpFullPath.c_str());
    QString currentDir(gvpFileInfo.absolutePath() + "/");
    QString prjDataDir(currentDir + gvpFileInfo.baseName() + "/");

    if (gvpFullPath.empty())
        return false;

    QFile gvpFile(gvpFullPath.c_str());
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
    QDomNodeList listOfRootNode = root.childNodes();
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
            Model model;
            model.modelName = childElement.text();
            m_objManager.treeOfGeoObjs.push_back(model);
        }
    }
    gvpFile.close();

    for (int i = 0; i < m_objManager.treeOfGeoObjs.size(); ++i)
    {
        QString modelName = m_objManager.treeOfGeoObjs[i].modelName;
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

bool GVSDoc::parseAndLoadModel(std::string gvmFullPath)
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
        if (0 == iter_model->modelName.compare(gvmFileInfo.baseName()))
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

        //reader
        vtkDataSetReader* reader = vtkDataSetReader::New();
        reader->SetFileName((modelDataDir + objFileName).toStdString().c_str());
        //reader->Update();

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

        GeoObject geoObj(objFileName.toStdString(), objFileTy, reader, vis);
        //insert GeoObject into tree
        iter_model->vecOfGeoObjs.push_back(geoObj);

    }
    gvmFile.close();

    return true;
}

std::string GVSDoc::getProjectPathByDlg()
{
    QString fileName = QFileDialog::getOpenFileName(
                               0,
                               tr("打开项目文件"),
                               tr("D:\\"),
                               tr("所有文件(*.*);;GVP项目文件(*.gvp)"),
                               &tr("GVP项目文件(*.gvp)"));

    return fileName.toStdString();
}


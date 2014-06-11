#include "GVSDoc.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>

GVSDoc::GVSDoc(MainWindow* mainWindow, QObject *parent)
    : QObject(parent)
{
    m_pMainWindow = m_pMainWindow;
    m_numOfObjects = 0;
    m_gvpFullFileName = "";
}

GVSDoc::~GVSDoc()
{
}

bool GVSDoc::OnOpenProject()
{
    string path = getProjectPath();
    if (path.empty() || !path.compare(m_gvpFullFileName))
    {
        return false;
    }
    m_gvpFullFileName = path;
    m_objectsManager.ClearObjectsTable();
    m_objectsManager.m_docsNameVector.clear();
    return LoadProjectFile(m_gvpFullFileName);
}

void GVSDoc::OnCloseDocument()
{
    m_objectsManager.ClearObjectsTable();
}

bool GVSDoc::LoadProjectFile(std::string gvpFullFileName)
{
    if (!LoadDocsNameOfProject(gvpFullFileName))
    {
        return false;
    }
    m_objectsManager.ReadObjectNames();
    m_objectsManager.UpdateAllReaders();
    m_objectsManager.ComputeBounds();
    return true;
}

bool GVSDoc::LoadDocsNameOfProject( std::string gvpFullFileName )
{
    m_objectsManager.m_docsNameVector.clear();
    bool ret = parseProjectObject(gvpFullFileName);

    m_numOfObjects=m_objectsManager.m_docsNameVector.size();
    return ret;
}

bool GVSDoc::parseProjectObject( std::string gvpFullFileName )
{
    //Locate the model files dir of project.
    QString strProjectFileFullPath(gvpFullFileName.c_str());
    QFileInfo projectFileInfo(strProjectFileFullPath);
    QString projectName = projectFileInfo.fileName();
    QString baseName = projectFileInfo.baseName();
    QString projectDir = projectFileInfo.absolutePath();
    projectDir.append("/");
    QString modelFilesDir = projectDir + baseName + "/";

    //parse xml, get the model file name and combine with modelFilesDir
    if(gvpFullFileName.empty())
        return false;

    QFile file(gvpFullFileName.c_str());
    if( !file.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::information(NULL, tr("打开项目失败"), file.errorString());
        return false;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    if( !domDoc.setContent(&file, false, &strError, &errLine, &errCol) ) {
        QMessageBox::information(NULL, tr("打开项目失败"), tr("非法项目文件"));
        file.close();
        return false;
    }

    if( domDoc.isNull() ) {
        QMessageBox::information(NULL, tr("打开项目失败"), tr("项目为空"));
        file.close();
        return false;
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
        if (0 != tagChild.compare("Models"))
        {
            continue;
        }
        else 
        {
            QDomNodeList nodeListOfModel = childOfProject.childNodes();
            int countModels = nodeListOfModel.count();
            for (int j = 0; j < countModels; ++j)
            {
                QDomNode modelNode = nodeListOfModel.item(j);
                QDomElement elementModel = modelNode.toElement();

                //Note that the field may be not only one field named "name",
                //so the field of model should be iterated.
                QDomNodeList fieldListOfModel = elementModel.childNodes();
                int countField = fieldListOfModel.count();
                for (int k = 0; k < countField; ++k)
                {
                    //name filed
                    QDomNode nameNode = fieldListOfModel.item(k);
                    QDomElement nameElement = nameNode.toElement();
                    string strModelName(modelFilesDir.toStdString());
                    //combine the dir and file name.
                    strModelName += nameElement.text().toStdString();
                    m_objectsManager.m_docsNameVector.push_back(strModelName);
                }
            }
            break;
        }
    }
    file.close();

    return true;
}

std::string GVSDoc::getProjectPath()
{
    QString fileName = QFileDialog::getOpenFileName(
                               0,
                               tr("打开项目文件"),
                               tr("D:\\vtkModel\\pieces"),
                               tr("所有文件(*.*);;GVP项目文件(*.gvp)"),
                               &tr("GVP项目文件(*.gvp)"));

    return fileName.toStdString();
}

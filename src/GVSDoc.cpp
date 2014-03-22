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
    LoadProjectFile(m_gvpFullFileName);
    return true;
}

void GVSDoc::OnCloseDocument()
{
    m_objectsManager.ClearObjectsTable();
}

void GVSDoc::LoadProjectFile(std::string gvpFullFileName)
{
    LoadDocsNameOfProject(gvpFullFileName);

    m_objectsManager.ReadAllDocsIntoReaders();
    m_objectsManager.UpdateAllReaders();
    m_objectsManager.ComputeBounds();
}

void GVSDoc::LoadDocsNameOfProject( std::string gvpFullFileName )
{
    m_objectsManager.m_docsNameVector.clear();
    parseProjectObject(gvpFullFileName);

    m_numOfObjects=m_objectsManager.m_docsNameVector.size();
}

void GVSDoc::parseProjectObject( std::string gvpFullFileName )
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
        return;

    QFile file(gvpFullFileName.c_str());
    if( !file.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::information(NULL, tr("����Ŀʧ��"), file.errorString());
        return;
    }

    QDomDocument domDoc;
    QString strError;
    int errLine = 0, errCol = 0;
    if( !domDoc.setContent(&file, false, &strError, &errLine, &errCol) ) {
        QMessageBox::information(NULL, tr("����Ŀʧ��"), tr("�Ƿ���Ŀ�ļ�"));
        file.close();
        return;
    }

    if( domDoc.isNull() ) {
        QMessageBox::information(NULL, tr("����Ŀʧ��"), tr("��ĿΪ��"));
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

    return;
}

std::string GVSDoc::getProjectPath()
{
    QString fileName = QFileDialog::getOpenFileName(
                               0,
                               tr("����Ŀ�ļ�"),
                               tr("D:\\vtkModel\\pieces"),
                               tr("�����ļ�(*.*);;GVP��Ŀ�ļ�(*.gvp)"),
                               &tr("GVP��Ŀ�ļ�(*.gvp)"));

    return fileName.toStdString();
}

#include "GVSDoc.h"
#include <QFileDialog>

GVSDoc::GVSDoc(MainWindow* mainWindow, QObject *parent)
    : QObject(parent)
{
    m_pMainWindow = m_pMainWindow;
    m_numOfObjects = 0;
    m_pathNameOfProject = "";
}

GVSDoc::~GVSDoc()
{
}

bool GVSDoc::OnOpenProject()
{
    string path = getProjectPath();
    if (path.empty() || !path.compare(m_pathNameOfProject))
    {
        return false;
    }
    m_pathNameOfProject = path;
    m_objectsManager.ClearObjectsTable();
    m_objectsManager.m_docsNameVector.clear();
    LoadProjectFile(m_pathNameOfProject);
    return true;
}

void GVSDoc::OnCloseDocument()
{
    m_objectsManager.ClearObjectsTable();
}

void GVSDoc::LoadProjectFile(std::string projectFileName)
{
    LoadDocsNameOfProject(projectFileName);

    m_objectsManager.ReadAllDocsIntoReaders();
    m_objectsManager.UpdateAllReaders();
    m_objectsManager.ComputeBounds();
    m_numOfObjects=m_objectsManager.m_docsNameVector.size();
}

void GVSDoc::LoadDocsNameOfProject( std::string projectFileName )
{
    m_objectsManager.m_docsNameVector = parseProjectObject(projectFileName);

    m_numOfObjects=m_objectsManager.m_docsNameVector.size();
}

vector<string> GVSDoc::parseProjectObject( std::string projectFileName )
{
    std::vector<std::string> names;

    //TODO implement the process of parsing project file.

    //mock the project names
    names.push_back("D:/vtkModel/pieces/Ar2c1.vtk");
    names.push_back("D:/vtkModel/pieces/Ar2c2.vtk");
    names.push_back("D:/vtkModel/pieces/Ar2d1.vtk");
    names.push_back("D:/vtkModel/pieces/bj.vtk");
    names.push_back("D:/vtkModel/pieces/dem.vtk");
    names.push_back("D:/vtkModel/pieces/di.vtk");
    names.push_back("D:/vtkModel/pieces/Ej-m2.vtk");
    names.push_back("D:/vtkModel/pieces/Eoy-l2.vtk");
    names.push_back("D:/vtkModel/pieces/Ez-cm2.vtk");
    names.push_back("D:/vtkModel/pieces/F1.vtk");
    names.push_back("D:/vtkModel/pieces/F2.vtk");
    names.push_back("D:/vtkModel/pieces/F3.vtk");
    names.push_back("D:/vtkModel/pieces/F4.vtk");
    names.push_back("D:/vtkModel/pieces/F5.vtk");
    names.push_back("D:/vtkModel/pieces/Om2.vtk");
    names.push_back("D:/vtkModel/pieces/Qnd1.vtk");

    return names;
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

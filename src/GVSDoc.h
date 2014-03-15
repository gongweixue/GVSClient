#ifndef GVSDOC_H
#define GVSDOC_H

#include <QObject>
#include <string>
#include <Managers/vtkObjectsManager.h>

class MainWindow;

class GVSDoc : public QObject
{
    Q_OBJECT

public:
    GVSDoc(MainWindow* mainWindow, QObject *parent = 0);
    ~GVSDoc();
    void OnCloseDocument();
    ObjectsManager* GetObjectsManager(){return &m_objectsManager;};

public slots:
    void OnOpenProject();

private:
    void LoadDocsNameOfProject(std::string projectFileName);
    void LoadProjectFile(std::string projectFileName);
    vector<string> parseProject(std::string projectFileName);
    std::string getProjectPath();
private:
    MainWindow* m_pMainWindow;//pointer to view
    ObjectsManager m_objectsManager;
    int m_numOfObjects;
};

#endif // GVSDOC_H

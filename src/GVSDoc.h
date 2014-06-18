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
    ObjectManager* GetObjectsManager(){return &m_objManager;}
    string getProjectPathName() { return m_gvpFullFileName; }

public slots:
    bool OnOpenProject();

private:
    bool LoadProject(std::string gvpFullFileName);
    bool parseProjectByFileName(std::string gvpFullFileName);
    std::string getProjectPathByDlg();
    bool parseAndLoadModel(std::string gvmFullPath);
private:
    MainWindow* m_pMainWindow;
    ObjectManager m_objManager;
    std::string m_gvpFullFileName;
};

#endif // GVSDOC_H

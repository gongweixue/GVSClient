#ifndef GVS_DOC_H
#define GVS_DOC_H

#include <string>
#include <QFileInfo>
#include <QObject>
#include <Managers/vtkObjectsManager.h>

class MainWindow;

class GVSDoc : public QObject
{
    Q_OBJECT

public:
    GVSDoc(MainWindow* mainWindow, QObject *parent = 0);
    ~GVSDoc();
    ObjectManager* GetObjManager(){ return &m_objManager; }
    string getProjectPathName() { return m_gvpFullFileName; }
    bool setObjVisByName(QString modelName, QString objName, bool vis);

public slots:
    bool OnOpenProject();
    bool OnCloseDocument();

private:
    bool LoadProject(std::string gvpFullFileName);
    bool parseProjectByFileName(std::string gvpFullFileName);
    std::string getProjectPathByDlg();
    bool parseAndLoadModel(std::string gvmFullPath);
    bool LoadFavTree(QFileInfo fileInfo);
private:
    MainWindow* m_pMainWindow;
    ObjectManager m_objManager;
    std::string m_gvpFullFileName;
};

#endif // GVS_DOC_H

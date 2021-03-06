#ifndef GVS_DOC_H
#define GVS_DOC_H

#include <string>
#include <QFileInfo>
#include <QObject>
#include <Managers/vtkObjectsManager.h>

class MainWindow;

class GVSDoc : public QObject {
    Q_OBJECT

public:
    GVSDoc(MainWindow* mainWindow, QObject* parent = 0);
    ~GVSDoc();
    ObjectManager* GetObjManager() { return &m_objManager; }
    string getProjectPathName() { return m_gvpFullFileName; }

public slots:
    bool OnOpenProject();
    bool OnCloseDocument();

private:
    bool LoadProject(const std::string& gvpFullFileName);
    bool parseProjectByFileName(const std::string& gvpFullPath);
    std::string getProjectPathByDlg();
    bool parseAndLoadModel(const std::string& gvmFullPath);
    bool LoadFavTree(const std::string& filePath);
    bool LoadObjTree(const std::string& filePath);

private:
    MainWindow* m_pMainWindow;
    ObjectManager m_objManager;
    std::string m_gvpFullFileName;
};

#endif // GVS_DOC_H

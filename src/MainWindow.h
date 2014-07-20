#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QLabel>
#include <QSplitter>
#include <QTabWidget>
#include <QtGui/QMainWindow>
#include <QTreeWidget>
#include <QVTKWidget.h>
#include "ui_MainWindow.h"
#include "Callbacks/vtkBoxClipCallback.h"
#include "Callbacks/vtkPlaneClipCallback.h"
#include "Extention/GVSPrjTreeWidget.h"
#include "Extention/GVSPrjTreeWidgetItem.h"
#include "Managers/ColorLegendManager.h"
#include "Managers/vtkSceneManager.h"

class GVSDoc;
class TransportationManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();
    void welcomeYou();
    GVSDoc* getDocument();
    vtkRenderer* getRenderer() {return m_mainRenderer;}
    void removeAllActorsOfRenderer(vtkRenderer* renderer);
    SceneManager* getSceneManager() {return &m_sceneManager;}
    QVTKWidget* getQVTKWidget() { return qvtkWidget;}

private:
    int loadLocalConfig();
    int loadNetwork();
    void initMembers();
    void initCommonMembers();
    void destoryCommonMembers();
    void destoryMembers();
    void initStatusBarMembers();
    void initMainAreaMembers();
    void destoryStatusBarMembers();
    void destoryMainAreaMembers();
    void bindingActionsWithSlots();

    void initPrjExplorer();
    void initObjectItems();
    void initFavItems();
//     void updateFavItem(QTreeWidgetItem* favItem);
     void updateObjItem(QString modelName, QString objName, bool objVisible);

    void prjExplorerObjItemClicked(GVSPrjTreeWidgetItem* item);
    void prjExplorerFavItemClicked(GVSPrjTreeWidgetItem* item);
    void prjExplorerFavGroupClicked(GVSPrjTreeWidgetItem* item_clicked);
    void prjExplorerModelClicked(GVSPrjTreeWidgetItem* item_clicked);
    void refreshGroupCheckState(QTreeWidgetItem* groupWidget);
    void refreshModelCheckState(QTreeWidgetItem* modelWidget);

    QTreeWidgetItem* findObjItemInPrjTree(QString modelName, QString objName);
    QTreeWidgetItem* findFavItemInPrjTree(QString groupName, QString favName);

    bool setActorVisByName(QString actorName, bool vis);

    void onInitialUpdate();
    void showOrientationMarker();
    void processRenderRequest(int state);
    void RenderOriginal();
    void RenderPlaneClip();
    void RenderStdExplode();
    void RenderBoxClip();
    void RenderPrismClip();
    vtkActor* MapToActor(vtkDataSet* ds);
    void TurnCubeAxesOnOff(int isOn, int xGridOn, int yGridOn);

    MainWindow(const MainWindow&); // Not implemented.
    void operator=(const MainWindow&); // Not implemented.

public slots:
    void OnOpenProject();
    void OnRenderClip();
    void OnCrossExplode();
    void OnRenderBoxClip();
    void OnBoxClipWidgetOnOff();
    void OnPrismClip();
    void OnPrismClipWidgetOnOff();
    void OnRenderOriginal();
    void OnQuickTurnCamLight();
    void OnQuickTurnSceneLight();
    void OnOrientationOnOff();
    void OnTurnCubeAxesOnOff();
    void OnLightOption();
    void OnCubeAxesOption();
    void OnStdExplode();
    void OnShowColorLegend();
    void OnProjectExplorer();
    void OnEditColorLegend();
    void OnPrjExplorerTreeItemClicked(QTreeWidgetItem* item, int column);
    void OnChangingObjColor(QString& modelName, QString& objName);
    void OnAddFavGroup();
    void OnAddFavItem(GVSPrjTreeWidgetItem& currTreeItem);
    void OnRemoveGroup(GVSPrjTreeWidgetItem& itemRemoved);
public:
    Ui::MainWindowClass ui;

private:
    QSplitter* splitterMain;
    QTabWidget* m_ProjectExplorer;
    //QTreeWidget* m_prjTreeWidget;
    GVSPrjTreeWidget* m_prjTreeWidget;

    QVTKWidget* qvtkWidget;
    GVSDoc* m_pDoc;

    vtkOrientationMarkerWidget* m_OrientationMarker;

    vtkRenderer* m_mainRenderer;

    vtkPlane* m_clipPlane;
    vtkPlaneClipCallback* m_clipCallback;
    vtkImplicitPlaneRepresentation* m_clipPlaneRep;
    vtkImplicitPlaneWidget2* m_clipPlaneWidget;

    int m_evenExplodeRow;
    int m_evenExplodeCol;
    double m_evenExplodeGapRatio;

    vtkOpenGLLight* m_camLight;
    vtkOpenGLLight* m_sceneLightUp;
    vtkOpenGLLight* m_sceneLightDown;

    vtkCubeAxesActor* m_cubeAxesActor;

    vtkBoxWidget* m_boxClipWidget;
    vtkBoxClipCallback* m_boxClipCallback;
    vtkPlanes* m_boxClipPlanes;
    vtkPlane* m_boxClipPlane[6];

    vtkPlane* m_prismClipPlane[3];
    vtkImplicitPlaneRepresentation* m_prismClipRep[3];
    vtkImplicitPlaneWidget2* m_prismClipWidget[3];
    vtkPlaneClipCallback* m_prismClipCallback[3];

    SceneManager m_sceneManager;
    ColorLegendManager* m_ColorLegendManager;
    TransportationManager* m_transManager;
};

#endif // MAINWINDOW_H

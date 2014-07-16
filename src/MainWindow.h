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
    SceneManager* getSceneManager(){return &m_sceneManager;}

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
    void fillUpPrjExplorer();
    void fillUpObjects();
    void fillUpFav();
    void UpdateObjItem(QString modelName, QString objName, bool objVisible);
    bool setActorVisByName(QString actorName, bool vis);

    MainWindow(const MainWindow&); // Not implemented.
    void operator=(const MainWindow&); // Not implemented.

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
    void OnPrjExplorerObjItemClicked(QTreeWidgetItem* item, int column);

public:
    Ui::MainWindowClass ui;

private:
    QSplitter* splitterMain;
    QTabWidget* m_ProjectExplorer;
    QTreeWidget* m_prjTreeWidget;

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

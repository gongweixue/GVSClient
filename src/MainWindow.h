#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QSplitter>
#include <QtGui/QMainWindow>
#include <QTabWidget>
#include <QTreeView>
#include <QVTKWidget.h>
#include "ui_MainWindow.h"

#include "Managers/vtkSceneManager.h"
#include "Callbacks/vtkBoxClipCallback.h"
#include "Callbacks/vtkPlaneClipCallback.h"

class GVSDoc;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();
    void displayCube();
    GVSDoc* getDocument();
    vtkRenderer* getRenderer() { return m_mainRenderer; };
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
    vtkActor* MappingDataSetToActor(vtkDataSet* ds);
    void TurnCubeAxesOnOff(int isOn, int xGridOn, int yGridOn);

//public slots:
    
    void OnStdExplode();
    void OnLightOption();
    void OnCubeAxesOption();
    void OnTurnCubeAxesOnOff();
    void OnOrientationOnOff();
    void OnQuickTurnCamLight();
    void OnQuickTurnSceneLight();
    
    

public slots:
    void OnOpenProject();
    void OnRenderClip();
    void OnCrossExplode();
    void OnRenderBoxClip();
    void OnBoxClipWidgetOnOff();
    void OnPrismClip();
    void OnPrismClipWidgetOnOff();
    void OnRenderOriginal();

private:
    Ui::MainWindowClass ui;
    QLabel* statusBartipLabel;
    QLabel* statusBartipLabel2;
    QSplitter* splitterMain;
    QTabWidget* leftTab;
    QTreeView* treeTab1;
    QTreeView* treeTab2;
    QVTKWidget* qvtkWidget;
    GVSDoc* m_pDoc;

    vtkOrientationMarkerWidget* m_OrientationMarker;

    vtkRenderer* m_mainRenderer;
    SceneManager m_sceneManager;

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
};

#endif // MAINWINDOW_H

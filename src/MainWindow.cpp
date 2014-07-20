#include <stdio.h>
#include <stdlib.h>
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkCaptionActor2D.h>
#include <vtkCellData.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkFloatArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <QColorDialog>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTextEdit>
#include <QVBoxLayout>
#include "ContextMenus/AddFavGroupDialog.h"
#include "ContextMenus/AddFavItemDialog.h"
#include "Extention/GVSCompassActor.h"
#include "Extention/GVSPrjTreeWidgetItem.h"
#include "GVSDoc.h"
#include "Managers/TransportationManager.h"
#include "MainWindow.h"
#include "Options/ColorLegendEditor.h"
#include "Options/CubeAxesOption.h"
#include "Options/LightOption.h"
#include "Options/StdExplode.h"

//#define GVS_SHOW_SPLASH

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    //show startup image while preparing.
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

#ifdef GVS_SHOW_SPLASH
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/Resources/startup.jpg"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("对象初始化"), topRight, Qt::red);
    Sleep(1000);
#endif

    initMembers();
    bindingActionsWithSlots();

#ifdef GVS_SHOW_SPLASH
    splash->showMessage(QObject::tr("加载本地配置"), topRight, Qt::red);
    Sleep(1000);
#endif
    loadLocalConfig();
    loadNetwork();
#ifdef GVS_SHOW_SPLASH
    splash->showMessage(QObject::tr("设置网络"), topRight, Qt::red);
    Sleep(1000);
    splash->clearMessage();
    splash->close();
#endif
}

MainWindow::~MainWindow()
{
    destoryMembers();
}

int MainWindow::loadLocalConfig()
{
    //TODO: load some config and return the status code:
    //0 is ok and other value means there are some errors, then abort.

    return 0;
}

int MainWindow::loadNetwork()
{
    //TODO: load some network info and return a code.
    //0 is ok and other value is error, and give a tip to user.

    return 0;
}

void MainWindow::initMembers()
{
    ui.setupUi(this);

    initStatusBarMembers();
    initMainAreaMembers();
    initCommonMembers();
}

void MainWindow::destoryMembers()
{
    destoryCommonMembers();

    destoryStatusBarMembers();
    destoryMainAreaMembers();
}

void MainWindow::initStatusBarMembers()
{
    QStatusBar* statBar = this->statusBar();
}

void MainWindow::initMainAreaMembers()
{
    splitterMain = new QSplitter(Qt::Horizontal);
    splitterMain->setParent(this);

    //left obj explorer init
    m_ProjectExplorer = new QTabWidget(splitterMain);
    m_prjTreeWidget = new GVSPrjTreeWidget(m_ProjectExplorer);
    m_prjTreeWidget->setHeaderLabel(tr("无项目..."));
    QIcon tabIcon(QString(":/Resources/PrjExplorer/TabIcon.png"));
    m_ProjectExplorer->addTab(m_prjTreeWidget, tabIcon, QString(tr("对象浏览")));

    //right side init
    qvtkWidget = new QVTKWidget(splitterMain);

    splitterMain->setStretchFactor(1, 1);
    setCentralWidget(splitterMain);

    this->ui.dockColorLegend->setVisible(false);
}

void MainWindow::initCommonMembers()
{
    m_pDoc = new GVSDoc(this);
    m_mainRenderer = vtkRenderer::New();
    m_OrientationMarker = vtkOrientationMarkerWidget::New();

    m_clipPlane = vtkPlane::New();
    m_clipPlaneRep = vtkImplicitPlaneRepresentation::New();
    m_clipPlaneWidget = vtkImplicitPlaneWidget2::New();
    m_clipCallback = vtkPlaneClipCallback::New();

    m_evenExplodeRow = 1;
    m_evenExplodeCol = 1;
    m_evenExplodeGapRatio = 1;

    m_camLight = vtkOpenGLLight::New();
    m_camLight->SetLightTypeToCameraLight();
    m_camLight->SetSwitch(1);
    m_camLight->SetIntensity(1);
    m_mainRenderer->AddLight(m_camLight);
    m_sceneLightUp = vtkOpenGLLight::New();
    m_sceneLightUp->SetLightTypeToSceneLight();
    m_sceneLightUp->SetSwitch(1);
    m_sceneLightUp->SetIntensity(1);
    m_sceneLightUp->SetPosition(1, 1, 1);
    m_mainRenderer->AddLight(m_sceneLightUp);
    m_sceneLightDown = vtkOpenGLLight::New();
    m_sceneLightDown->SetLightTypeToSceneLight();
    m_sceneLightDown->SetSwitch(1);
    m_sceneLightDown->SetIntensity(1);
    m_sceneLightDown->SetPosition(-1, -1, -1);
    m_mainRenderer->AddLight(m_sceneLightDown);

    m_cubeAxesActor = vtkCubeAxesActor::New();
    m_cubeAxesActor->DrawXGridlinesOn();
    m_cubeAxesActor->DrawYGridlinesOn();
    m_cubeAxesActor->SetVisibility(0);

    m_boxClipWidget = vtkBoxWidget::New();
    m_boxClipCallback = vtkBoxClipCallback::New();
    m_boxClipPlanes = vtkPlanes::New();
    for (int i = 0; i < 6; i++)
    {
        m_boxClipPlane[i] = vtkPlane::New();
    }

    for (int i = 0; i < 3; i++)
    {
        m_prismClipPlane[i] = vtkPlane::New();
        m_prismClipWidget[i] = vtkImplicitPlaneWidget2::New();
        m_prismClipRep[i] = vtkImplicitPlaneRepresentation::New();
        m_prismClipCallback[i] = vtkPlaneClipCallback::New();
    }

    m_ColorLegendManager = new ColorLegendManager(this->ui.legendItemList,
                                                  m_pDoc->getProjectPathName());
    ui.dockColorLegend->setWidget(ui.legendItemList);
    ui.dockColorLegend->setWindowTitle(tr("图例"));

    m_transManager = new TransportationManager(this, 0);
}

void MainWindow::destoryStatusBarMembers()
{
//     delete statusBartipLabel;
//     delete statusBartipLabel2;
}

void MainWindow::destoryMainAreaMembers()
{
    delete qvtkWidget;
    delete splitterMain;
}

void MainWindow::destoryCommonMembers()
{
    DeleteVTKPointer(m_OrientationMarker);
    DeleteVTKPointer(m_clipPlane);
    DeleteVTKPointer(m_clipPlaneRep);
    DeleteVTKPointer(m_clipPlaneWidget);
    DeleteVTKPointer(m_clipCallback);

    DeleteVTKPointer(m_camLight);
    DeleteVTKPointer(m_sceneLightUp);
    DeleteVTKPointer(m_sceneLightDown);

    DeleteVTKPointer(m_cubeAxesActor);

    DeleteVTKPointer(m_boxClipWidget);
    DeleteVTKPointer(m_boxClipCallback);
    DeleteVTKPointer(m_boxClipPlanes);
    for (int i = 0; i < 6; i++)
    {
        DeleteVTKPointer(m_boxClipPlane[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        DeleteVTKPointer(m_prismClipPlane[i]);
        DeleteVTKPointer(m_prismClipWidget[i]);
        DeleteVTKPointer(m_prismClipRep[i]);
        DeleteVTKPointer(m_prismClipCallback[i]);
    }

    removeAllActorsOfRenderer(m_mainRenderer);
    m_sceneManager.ClearActorTable();

    DeleteVTKPointer(m_mainRenderer);

    delete m_pDoc;
    delete m_ColorLegendManager;
    delete m_transManager;
}

void MainWindow::welcomeYou()
{
    vtkRenderer* renderer = this->m_mainRenderer;
    qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    renderer->SetBackground(.32, .34, .43);
    qvtkWidget->GetRenderWindow()->Render();
    showOrientationMarker();
}

GVSDoc* MainWindow::getDocument()
{
    return m_pDoc;
}

void MainWindow::onInitialUpdate()
{
    m_sceneManager.SetSceneState(SCENE_STATE_NULL);
    m_mainRenderer->SetBackground(0.32, 0.34, 0.43);
    m_mainRenderer->ResetCamera();
    m_mainRenderer->AutomaticLightCreationOff();
    m_mainRenderer->RemoveAllViewProps();
    m_sceneManager.ClearActorTable();
    m_mainRenderer->Render();
    //////////////////////
    if (m_pDoc->GetObjManager()->getNumOfObjsInTree() != 0)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_ORIGINAL);
        //showOrientationMarker();//avoid mem leak, just invoke once at func 'welcomeYou'
        processRenderRequest(m_sceneManager.GetSceneState());
    }
    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::showOrientationMarker()
{
    m_OrientationMarker->SetInteractor(qvtkWidget->GetInteractor());

    vtkSmartPointer<GVSCompassActor> gvsCompass = vtkSmartPointer<GVSCompassActor>::New();
    m_OrientationMarker->SetOrientationMarker(gvsCompass);
    m_OrientationMarker->SetEnabled(1);
    m_OrientationMarker->SetViewport(1-0.23, 0, 1, 0.23);
    m_OrientationMarker->KeyPressActivationOff();
    m_OrientationMarker->InteractiveOff();

    double backColor[3];
    m_mainRenderer->GetBackground(backColor);
    m_OrientationMarker->SetOutlineColor(backColor[0], backColor[1], backColor[2]);
}

void MainWindow::removeAllActorsOfRenderer(vtkRenderer* renderer)
{
    renderer->RemoveAllViewProps();
}

void MainWindow::processRenderRequest(int state)
{
    removeAllActorsOfRenderer(m_mainRenderer);
    m_sceneManager.ClearActorTable();
    GVSDoc* pDoc = getDocument();
    if (!pDoc)
        return;
    else
    {
        if (pDoc->GetObjManager()->getNumOfObjsInTree() != 0)
        {
            switch (state)
            {
            case SCENE_STATE_ORIGINAL:
                RenderOriginal();
                break;
            case SCENE_STATE_PLANE_CLIP:
                RenderPlaneClip();
                break;
            case SCENE_STATE_STD_EXPLODE:
                RenderStdExplode();
                break;
            case SCENE_STATE_BOX_CLIP:
                RenderBoxClip();
                break;
            case SCENE_STATE_PRISM_CLIP:
                RenderPrismClip();
                break;
            default:
                throw std::exception("Wrong state value.");
            }
        }
    }
    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::RenderOriginal()
{
    if (0 == getDocument()->GetObjManager()->getNumOfObjsInTree())
    {
        return;
    }

    m_sceneManager.SetSceneBounds(getDocument()->GetObjManager()->m_bounds);
    vector<Model>::iterator model_iter =
            m_pDoc->GetObjManager()->treeOfGeoObjs.begin();
    for ( ; model_iter != m_pDoc->GetObjManager()->treeOfGeoObjs.end(); model_iter++)
    {
        string modelName = model_iter->name.toStdString();
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            //in case of the name including more than 1 '.', like "abc.def.vtk".
            string objName = obj_iter->getName().toStdString();
            QStringList spltList = QString(objName.c_str()).split('.');
            string actorName = modelName + "/" + objName;
            m_sceneManager.InsertActorRcrd(MapToActor(obj_iter->reader->GetOutput()),
                                          actorName,
                                          SCENE_STATE_ORIGINAL,
                                          obj_iter->getVisibility());
        }
    }

    m_sceneManager.AddCrrtStatActrToRnder(m_mainRenderer);
    m_mainRenderer->ResetCamera();
    m_mainRenderer->Render();
}

void MainWindow::RenderPlaneClip()
{
    if (0 == this->m_pDoc->GetObjManager()->getNumOfObjsInTree())
    {
        return;
    }

    vtkSmartPointer<vtkAppendFilter> apdFilter=
            vtkSmartPointer<vtkAppendFilter>::New();

    vector<Model>::iterator model_iter = m_pDoc->GetObjManager()->treeOfGeoObjs.begin();
    for ( ; model_iter != m_pDoc->GetObjManager()->treeOfGeoObjs.end(); model_iter++)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            if (obj_iter->getVisibility())
            {
                obj_iter->reader->Update();
                apdFilter->AddInput(obj_iter->reader->GetOutput());
            }
        }
    }

    apdFilter->Update();
    vtkDataSet* mergedDataSet = apdFilter->GetOutput();

    m_clipPlane->SetNormal(1, 0, 0);
    vtkSmartPointer<vtkTableBasedClipDataSet> clipper =
            vtkSmartPointer<vtkTableBasedClipDataSet>::New();
    clipper->SetClipFunction(m_clipPlane);
    clipper->InsideOutOn();
    clipper->SetInput(mergedDataSet);
    clipper->Update();

    vtkActor* clipActor = MapToActor(clipper->GetOutput());
    m_sceneManager.InsertActorRcrd(clipActor,
                                     "_TempClipResult",
                                     SCENE_STATE_PLANE_CLIP,
                                     1);
    m_sceneManager.AddCrrtStatActrToRnder(m_mainRenderer);

    m_clipPlaneRep->SetPlaceFactor(1.25);
    double tmpBounds[6];
    mergedDataSet->GetBounds(tmpBounds);
    m_clipPlaneRep->PlaceWidget(tmpBounds);
    m_clipPlaneRep->SetNormal(m_clipPlane->GetNormal());
    m_clipPlaneRep->SetOrigin((tmpBounds[0]+tmpBounds[1]) / 2,
                              (tmpBounds[2]+tmpBounds[3]) / 2,
                              (tmpBounds[5]+tmpBounds[4]) / 2);
    m_clipPlaneRep->DrawPlaneOff();

    m_clipCallback->Plane = m_clipPlane;
    m_clipCallback->Actor = clipActor;

    m_clipPlaneWidget->SetInteractor(this->qvtkWidget->GetInteractor());
    m_clipPlaneWidget->SetRepresentation(m_clipPlaneRep);
    m_clipPlaneWidget->AddObserver(vtkCommand::InteractionEvent, m_clipCallback);
    m_clipPlaneWidget->On();
    m_clipPlaneWidget->EnabledOn();
    //reset camera
    m_mainRenderer->ResetCamera();
    m_mainRenderer->Render();
}

void MainWindow::RenderStdExplode()
{
    //append object data which visible=1.
    vtkSmartPointer<vtkAppendFilter> apdFilter = vtkSmartPointer<vtkAppendFilter>::New();
    vector<Model>::iterator model_iter = m_pDoc->GetObjManager()->treeOfGeoObjs.begin();
    for ( ; model_iter != m_pDoc->GetObjManager()->treeOfGeoObjs.end(); model_iter++)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            if (obj_iter->getVisibility())
            {
                obj_iter->reader->Update();
                apdFilter->AddInput(obj_iter->reader->GetOutput());
            }
        }
    }

    apdFilter->Update();
    vtkDataSet* mergedDataSet = apdFilter->GetOutput();
    //bounds
    double bounds[6];
    mergedDataSet->GetBounds(bounds);
    //gap size
    double explodeGap = m_evenExplodeGapRatio*(bounds[5] - bounds[4]);

    //--------------------------------horizon cut-------------------------------
    //temp var to store clipper, trans,etc.
    vtkSmartPointer<vtkDataSet> hResultData[evenRowLMT];
    vtkSmartPointer<vtkPlane> hPlane[evenRowLMT - 1];
    vtkSmartPointer<vtkTableBasedClipDataSet> hClipper[evenRowLMT - 1][2];
    vtkSmartPointer<vtkTransform> hTrans[evenRowLMT-1];
    vtkSmartPointer<vtkTransformFilter> hTransFilter[evenRowLMT - 1];
    //temp var to be process in next loop.
    vtkSmartPointer<vtkDataSet> inputData=mergedDataSet;
    // horizon cut begin
    for (int i = 0; i < (m_evenExplodeRow - 1); i++)
    {
        hPlane[i] = vtkSmartPointer<vtkPlane>::New();
        hPlane[i]->SetNormal(0, 1, 0);
        hPlane[i]->SetOrigin(0,
                             (bounds[3] - (i + 1) * (bounds[3] - bounds[2]) /
                             m_evenExplodeRow),
                             0);
        //clipper positive, insideOutOff
        hClipper[i][0] = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        hClipper[i][0]->SetInput(inputData);
        hClipper[i][0]->SetClipFunction(hPlane[i]);
        hClipper[i][0]->InsideOutOff();
        hClipper[i][0]->Update();
        //transFilter
        hTrans[i] = vtkSmartPointer<vtkTransform>::New();
        hTrans[i]->Translate(0, (m_evenExplodeRow - i - 1) * explodeGap, 0);
        hTransFilter[i] = vtkSmartPointer<vtkTransformFilter>::New();
        hTransFilter[i]->SetInputConnection(hClipper[i][0]->GetOutputPort());
        hTransFilter[i]->SetTransform(hTrans[i]);
        hTransFilter[i]->Update();
        //store into tmp var
        hResultData[i] = hTransFilter[i]->GetOutput();
        //clipper nagtive, insideOutOn
        hClipper[i][1] = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        hClipper[i][1]->SetInput(inputData);
        hClipper[i][1]->SetClipFunction(hPlane[i]);
        hClipper[i][1]->InsideOutOn();
        hClipper[i][1]->Update();
        //prepare for next loop
        inputData = hClipper[i][1]->GetOutput();
    }
    //store the last ClippedOutput in to hResultData.
    hResultData[m_evenExplodeRow-1] = inputData;

    //----------------------------------------vertical cut--------------------------
    //temp var to store clipper, trans,etc.
    vtkSmartPointer<vtkPlane> vPlane[evenRowLMT][evenColLMT - 1];
    vtkSmartPointer<vtkTableBasedClipDataSet> vClipper[evenRowLMT][evenColLMT - 1][2];
    vtkSmartPointer<vtkTransform> vTrans[evenRowLMT][evenColLMT - 1];
    vtkSmartPointer<vtkTransformFilter> vTransFilter[evenRowLMT][evenColLMT - 1];
    vtkSmartPointer<vtkDataSet> resultData[evenRowLMT][evenColLMT];
    //vertical cut begin
    for (int i = 0; i < m_evenExplodeRow; i++)
    {
        inputData = hResultData[i];//for every horizon result data.
        for (int j = 0; j < (m_evenExplodeCol - 1); j++)
        {
            vPlane[i][j] = vtkSmartPointer<vtkPlane>::New();
            vPlane[i][j]->SetNormal(1, 0, 0);
            vPlane[i][j]->SetOrigin((bounds[1] - (j + 1) * (bounds[1] - bounds[0]) /
                                    m_evenExplodeCol),
                                    0,
                                    0);
            //clipper positive insideOutOff
            vClipper[i][j][0] = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            vClipper[i][j][0]->SetInput(inputData);
            vClipper[i][j][0]->SetClipFunction(vPlane[i][j]);
            vClipper[i][j][0]->InsideOutOff();
            vClipper[i][j][0]->Update();
            //transFilter
            vTrans[i][j] = vtkSmartPointer<vtkTransform>::New();
            vTrans[i][j]->Translate((m_evenExplodeCol - j - 1) * explodeGap, 0, 0);
            vTransFilter[i][j] = vtkSmartPointer<vtkTransformFilter>::New();
            vTransFilter[i][j]->SetInputConnection(vClipper[i][j][0]->GetOutputPort());
            vTransFilter[i][j]->SetTransform(vTrans[i][j]);
            vTransFilter[i][j]->Update();
            //store into final data array.
            resultData[i][j] = vTransFilter[i][j]->GetOutput();
            //clipper nagtive, insideOutOn
            vClipper[i][j][1]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            vClipper[i][j][1]->SetInput(inputData);
            vClipper[i][j][1]->SetClipFunction(vPlane[i][j]);
            vClipper[i][j][1]->InsideOutOn();
            vClipper[i][j][1]->Update();
            //for next loop
            inputData = vClipper[i][j][1]->GetOutput();
        }
        resultData[i][m_evenExplodeCol - 1] = inputData;
    }

    //mapping data to actor
    for (int i = 0; i < m_evenExplodeRow; i++)
    {
        for (int j = 0; j < m_evenExplodeCol; j++)
        {
            vtkActor* tmpActor = MapToActor(resultData[i][j]);
            m_sceneManager.InsertActorRcrd(tmpActor,
                                             "_StdExplodeResult",
                                             SCENE_STATE_STD_EXPLODE,
                                             true);
        }
    }
    //add all actor which state=stdExplode
    m_sceneManager.AddActorsByState(m_mainRenderer, SCENE_STATE_STD_EXPLODE);
    //render it.
    m_mainRenderer->ResetCamera();
    m_mainRenderer->Render();
}

void MainWindow::RenderPrismClip()
{
    if (0 == getDocument()->GetObjManager()->getNumOfObjsInTree())
    {
        return;
    }

    //append the object data which visible=1.
    vtkSmartPointer<vtkAppendFilter> apdFilter = vtkSmartPointer<vtkAppendFilter>::New();
    vector<Model>::iterator model_iter = m_pDoc->GetObjManager()->treeOfGeoObjs.begin();
    for ( ; model_iter != m_pDoc->GetObjManager()->treeOfGeoObjs.end(); model_iter++)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            if (obj_iter->getVisibility())
            {
                obj_iter->reader->Update();
                apdFilter->AddInput(obj_iter->reader->GetOutput());
            }
        }
    }

    apdFilter->Update();
    vtkSmartPointer<vtkAppendFilter> apdResult=
            vtkSmartPointer<vtkAppendFilter>::New();
    vtkDataSet* dataToClip = apdFilter->GetOutput();//appended result
    double bounds[6];
    dataToClip->GetBounds(bounds);
    for (int i = 0; i < 5; i++)
    {
        bounds[i] *= 1.1;
    }
    m_prismClipPlane[0]->SetOrigin(apdFilter->GetOutput()->GetCenter());
    m_prismClipPlane[0]->SetNormal(0, 0, 1);
    vtkSmartPointer<vtkTableBasedClipDataSet> clipper11=
            vtkSmartPointer<vtkTableBasedClipDataSet>::New();
    clipper11->SetClipFunction(m_prismClipPlane[0]);
    clipper11->SetInput(dataToClip);
    clipper11->InsideOutOn();
    clipper11->Update();
    apdResult->AddInput(clipper11->GetOutput());
    vtkTableBasedClipDataSet* clipper12 = vtkTableBasedClipDataSet::New();
    clipper12->SetClipFunction(m_prismClipPlane[0]);
    clipper12->SetInput(dataToClip);
    clipper12->InsideOutOff();
    clipper12->Update();
    dataToClip = clipper12->GetOutput();


    m_prismClipPlane[1]->SetOrigin(apdFilter->GetOutput()->GetCenter());
    m_prismClipPlane[1]->SetNormal(1, -1, 0);
    vtkTableBasedClipDataSet* clipper21 = vtkTableBasedClipDataSet::New();
    clipper21->SetClipFunction(m_prismClipPlane[1]);
    clipper21->SetInput(dataToClip);
    clipper21->InsideOutOn();
    clipper21->Update();
    apdResult->AddInput(clipper21->GetOutput());

    vtkTableBasedClipDataSet* clipper22 = vtkTableBasedClipDataSet::New();
    clipper22->SetClipFunction(m_prismClipPlane[1]);
    clipper22->SetInput(dataToClip);
    clipper22->InsideOutOff();
    clipper22->Update();
    dataToClip=clipper22->GetOutput();


    m_prismClipPlane[2]->SetOrigin(apdFilter->GetOutput()->GetCenter());
    m_prismClipPlane[2]->SetNormal(-1, -1, 0);
    vtkTableBasedClipDataSet* clipper31 = vtkTableBasedClipDataSet::New();
    clipper31->SetClipFunction(m_prismClipPlane[2]);
    clipper31->SetInput(dataToClip);
    clipper31->InsideOutOn();
    clipper31->Update();
    apdResult->AddInput(clipper31->GetOutput());
    apdResult->Update();

    vtkActor* tmpActor = MapToActor(apdResult->GetOutput() );
    m_sceneManager.InsertActorRcrd(tmpActor,
                                     "_PrismClipResult",
                                     SCENE_STATE_PRISM_CLIP,
                                     1);
    m_sceneManager.AddCrrtStatActrToRnder(m_mainRenderer);

    //----------------------------clip2------------------------------
    //rep2
    //m_prismClipRep[1]=vtkImplicitPlaneRepresentation::New();
    m_prismClipRep[1]->SetPlaceFactor(1);
    m_prismClipRep[1]->PlaceWidget(bounds);
    m_prismClipRep[1]->SetNormal(m_prismClipPlane[1]->GetNormal());
    m_prismClipRep[1]->SetOrigin(m_prismClipPlane[1]->GetOrigin());
    m_prismClipRep[1]->DrawPlaneOff();
    m_prismClipRep[1]->OutlineTranslationOff();
    m_prismClipRep[1]->OutsideBoundsOff();
    m_prismClipRep[1]->TubingOff();
    m_prismClipRep[1]->GetNormalProperty()->SetOpacity(0);
    m_prismClipRep[1]->GetOutlineProperty()->SetOpacity(0);
    m_prismClipRep[1]->GetSelectedOutlineProperty()->SetOpacity(0);
    //callback 2
    m_prismClipCallback[1]->Plane = m_prismClipPlane[1];
    //widget2
    m_prismClipWidget[1]->SetInteractor(qvtkWidget->GetInteractor());
    m_prismClipWidget[1]->SetRepresentation(m_prismClipRep[1]);
    m_prismClipWidget[1]->AddObserver(vtkCommand::EndInteractionEvent,
                                      m_prismClipCallback[1]);
    m_prismClipWidget[1]->On();
    m_prismClipWidget[1]->EnabledOn();
    //----------------------------clip3------------------------------
    //rep3
    m_prismClipRep[2]->SetPlaceFactor(1);
    m_prismClipRep[2]->PlaceWidget(bounds);
    m_prismClipRep[2]->SetNormal(m_prismClipPlane[2]->GetNormal());
    m_prismClipRep[2]->SetOrigin(m_prismClipPlane[2]->GetOrigin());
    m_prismClipRep[2]->DrawPlaneOff();
    m_prismClipRep[2]->OutlineTranslationOff();
    m_prismClipRep[2]->OutsideBoundsOff();
    m_prismClipRep[2]->TubingOff();
    m_prismClipRep[2]->GetNormalProperty()->SetOpacity(0);
    m_prismClipRep[2]->GetOutlineProperty()->SetOpacity(0);
    m_prismClipRep[2]->GetSelectedOutlineProperty()->SetOpacity(0);
    //callback2
    m_prismClipCallback[2]->Plane = m_prismClipPlane[2];
    //widget2
    m_prismClipWidget[2]->SetInteractor(qvtkWidget->GetInteractor());
    m_prismClipWidget[2]->SetRepresentation(m_prismClipRep[2]);
    m_prismClipWidget[2]->AddObserver(vtkCommand::EndInteractionEvent,
                                      m_prismClipCallback[2]);
    m_prismClipWidget[2]->On();
    m_prismClipWidget[2]->EnabledOn();
    //----------------------------clip1------------------------------
    m_prismClipRep[0]->SetPlaceFactor(1);
    m_prismClipRep[0]->PlaceWidget(bounds);
    m_prismClipRep[0]->SetNormal(m_prismClipPlane[0]->GetNormal());
    m_prismClipRep[0]->SetOrigin(m_prismClipPlane[0]->GetOrigin());
    m_prismClipRep[0]->DrawPlaneOff();
    m_prismClipRep[0]->OutlineTranslationOff();
    m_prismClipRep[0]->OutsideBoundsOff();
    m_prismClipRep[0]->GetNormalProperty()->SetOpacity(0);
    m_prismClipRep[0]->GetSelectedNormalProperty()->SetOpacity(0);
    m_prismClipRep[0]->GetOutlineProperty()->SetOpacity(0);
    m_prismClipRep[0]->GetSelectedOutlineProperty()->SetOpacity(0);

    m_prismClipCallback[0]->Plane = m_prismClipPlane[0];

    m_prismClipWidget[0]->SetInteractor(qvtkWidget->GetInteractor());
    m_prismClipWidget[0]->SetRepresentation(m_prismClipRep[0]);
    m_prismClipWidget[0]->AddObserver(vtkCommand::EndInteractionEvent,
                                      m_prismClipCallback[0]);
    m_prismClipWidget[0]->On();
    m_prismClipWidget[0]->EnabledOn();

    m_mainRenderer->ResetCamera();
    qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::RenderBoxClip()
{
    if (0 == getDocument()->GetObjManager()->getNumOfObjsInTree())
    {
        return;
    }

    vtkSmartPointer<vtkAppendFilter> apdFilter= vtkSmartPointer<vtkAppendFilter>::New();

    vector<Model>::iterator model_iter = m_pDoc->GetObjManager()->treeOfGeoObjs.begin();
    for ( ; model_iter != m_pDoc->GetObjManager()->treeOfGeoObjs.end(); model_iter++)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            if (obj_iter->getVisibility())
            {
                obj_iter->reader->Update();
                apdFilter->AddInput(obj_iter->reader->GetOutput());
            }
        }
    }

    apdFilter->Update();
    double bounds[6];
    apdFilter->GetOutput()->GetBounds(bounds);

    //box widget
    //m_boxClipWidget=vtkBoxWidget::New();
    m_boxClipWidget->SetInteractor(qvtkWidget->GetInteractor());
    m_boxClipWidget->SetPlaceFactor(1);
    m_boxClipWidget->SetInput(apdFilter->GetOutput());
    m_boxClipWidget->PlaceWidget(bounds[0],
                                 (bounds[0] + (bounds[1]-bounds[0]) / 4),
                                 bounds[2],
                                 (bounds[2] + (bounds[3]-bounds[2]) / 4),
                                 (bounds[4] + bounds[5]) / 2,
                                 (bounds[5] + (bounds[5] - bounds[4]) / 4));

    m_boxClipWidget->InsideOutOn();
    m_boxClipWidget->GetOutlineProperty()->SetRepresentationToWireframe();
    m_boxClipWidget->SetHandleSize(0.002);

    //planes
    m_boxClipWidget->GetPlanes(m_boxClipPlanes);
    for (int i = 0; i < 6; i++)
    {
        m_boxClipPlanes->GetPlane(i, m_boxClipPlane[i]);
    }
    //box clip result
    vtkSmartPointer<vtkDataSet> result[6];
    vtkSmartPointer<vtkTableBasedClipDataSet> clipper[6][2];
    vtkDataSet* dataToClip = apdFilter->GetOutput();
    //clip with 6 planes
    for (int i = 0; i < 6; i++)
    {
        clipper[i][0] = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        clipper[i][0]->SetInput(dataToClip);
        clipper[i][0]->SetClipFunction(m_boxClipPlane[i]);
        clipper[i][0]->InsideOutOn();
        clipper[i][0]->Update();
        result[i] = clipper[i][0]->GetOutput();

        clipper[i][1] = vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        clipper[i][1]->SetInput(dataToClip);
        clipper[i][1]->SetClipFunction(m_boxClipPlane[i]);
        clipper[i][1]->InsideOutOff();
        clipper[i][1]->Update();
        dataToClip = clipper[i][1]->GetOutput();
    }
    //append result
    vtkSmartPointer<vtkAppendFilter> apdResult = vtkSmartPointer<vtkAppendFilter>::New();
    for (int i = 0; i < 6; i++)
    {
        apdResult->AddInput(result[i]);
    }
    apdResult->Update();
    //callback
    m_boxClipCallback->boxPlaneArray = m_boxClipPlane;
    m_boxClipCallback->boxWidget = m_boxClipWidget;
    m_boxClipWidget->AddObserver(vtkCommand::EndInteractionEvent, m_boxClipCallback);
    //insert record
    vtkActor* tmpActor = MapToActor(apdResult->GetOutput());
    m_sceneManager.InsertActorRcrd(tmpActor, "_BoxClipActor", SCENE_STATE_BOX_CLIP, 1);
    //add actor to renderer
    m_mainRenderer->AddActor(tmpActor);
    //widget on
    m_boxClipWidget->On();
    m_boxClipWidget->EnabledOn();
}

vtkActor* MainWindow::MapToActor(vtkDataSet* ds)
{
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInput(ds);
    mapper->Update();
    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    return actor;
}

void MainWindow::TurnCubeAxesOnOff(int isOn, int xGridOn, int yGridOn)
{
    if (0 == getDocument()->GetObjManager()->getNumOfObjsInTree())
    {
        return;
    }

    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_cubeAxesActor->SetVisibility(isOn);
        m_cubeAxesActor->SetDrawXGridlines(xGridOn);
        m_cubeAxesActor->SetDrawYGridlines(yGridOn);
        m_cubeAxesActor->SetBounds(m_sceneManager.GetSceneBounds());
        m_cubeAxesActor->SetCamera(m_mainRenderer->GetActiveCamera());
        m_cubeAxesActor->SetTickLocationToOutside();
        m_cubeAxesActor->SetFlyModeToClosestTriad();
        if (isOn)
        {
            m_mainRenderer->AddActor(m_cubeAxesActor);
        }
        else
        {
            m_mainRenderer->RemoveActor(m_cubeAxesActor);
        }

        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::bindingActionsWithSlots()
{
    /***************************main menu and bar connections****************************/
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(OnOpenProject()));
    connect(ui.actionClipModel, SIGNAL(triggered()), this, SLOT(OnRenderClip()));
    connect(ui.actionCrossExplode, SIGNAL(triggered()), this, SLOT(OnCrossExplode()));
    connect(ui.actionCubeNotch, SIGNAL(triggered()), this, SLOT(OnRenderBoxClip()));
    connect(ui.actionShowHideBoxClipper, SIGNAL(triggered()),
            this, SLOT(OnBoxClipWidgetOnOff()));
    connect(ui.actionTriangleNotch, SIGNAL(triggered()), this, SLOT(OnPrismClip()));
    connect(ui.actionShowHideTriClipper, SIGNAL(triggered()),
            this, SLOT(OnPrismClipWidgetOnOff()));
    connect(ui.actionRenderOriginal, SIGNAL(triggered()), this, SLOT(OnRenderOriginal()));
    connect(ui.actionCamLight, SIGNAL(triggered()), this, SLOT(OnQuickTurnCamLight()));
    connect(ui.actionSceneLight, SIGNAL(triggered()),
            this, SLOT(OnQuickTurnSceneLight()));
    connect(ui.actionOrientatorCtrl, SIGNAL(triggered()),
            this, SLOT(OnOrientationOnOff()));
    connect(ui.actionRulerCtrl, SIGNAL(triggered()), this, SLOT(OnTurnCubeAxesOnOff()));
    connect(ui.actionRulerGridCtrl, SIGNAL(triggered()), this, SLOT(OnCubeAxesOption()));
    connect(ui.actionLightOption, SIGNAL(triggered()), this, SLOT(OnLightOption()));
    connect(ui.actionStdExplode, SIGNAL(triggered()), this, SLOT(OnStdExplode()));
    connect(ui.actionShowColorLegend, SIGNAL(triggered()),
            this, SLOT(OnShowColorLegend()));
    connect(ui.actionShowProjectExplorer, SIGNAL(triggered()),
            this, SLOT(OnProjectExplorer()));

    /**********************************color lenged stuff********************************/
    connect(ui.actionEditColorLegend, SIGNAL(triggered()),
        this, SLOT(OnEditColorLegend()));

    /*******************************prj tree connections*********************************/
    connect(m_prjTreeWidget, SIGNAL(sigChangeObjColor(QString&, QString&)),
            this, SLOT(OnChangingObjColor(QString&, QString&)));
    connect(m_prjTreeWidget, SIGNAL(sigAddFavGroup()), this, SLOT(OnAddFavGroup()));
    connect(m_prjTreeWidget, SIGNAL(sigAddFavItem(GVSPrjTreeWidgetItem&)),
            this, SLOT(OnAddFavItem(GVSPrjTreeWidgetItem&)));

}

void MainWindow::OnOpenProject()
{
    if (m_pDoc->OnOpenProject())
    {
        initPrjExplorer();
        onInitialUpdate();
        delete m_ColorLegendManager;
        m_ColorLegendManager = new ColorLegendManager(this->ui.legendItemList,
                                                      m_pDoc->getProjectPathName());
        m_ColorLegendManager->initOrUpdateLegend(m_pDoc->getProjectPathName());
    }
}

void MainWindow::OnRenderClip()
{
    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_PLANE_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_PLANE_CLIP);
    }
}

void MainWindow::OnCrossExplode()
{
    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_STD_EXPLODE);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();

        m_evenExplodeRow = 2;
        m_evenExplodeCol = 2;
        m_evenExplodeGapRatio = 1;
        processRenderRequest(SCENE_STATE_STD_EXPLODE);
    }
}

void MainWindow::OnRenderBoxClip()
{
    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_BOX_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_BOX_CLIP);
    }
}

void MainWindow::OnBoxClipWidgetOnOff()
{
    if (SCENE_STATE_BOX_CLIP == m_sceneManager.GetSceneState())
    {
        int isEnable = m_boxClipWidget->GetEnabled();
        m_boxClipWidget->SetEnabled(!isEnable);
        if (m_boxClipWidget->GetEnabled())
        {
            m_boxClipWidget->On();
        }
        else
        {
            m_boxClipWidget->Off();
        }
    }
    qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::OnPrismClip()
{
    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_PRISM_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_PRISM_CLIP);
    }
}

void MainWindow::OnPrismClipWidgetOnOff()
{
    if (SCENE_STATE_PRISM_CLIP == m_sceneManager.GetSceneState())
    {
        int isWidgetsOn = m_prismClipWidget[0]->GetEnabled();
        for (int i = 0; i < 3; i++)
        {
            m_prismClipWidget[i]->SetEnabled(!isWidgetsOn);
            if (m_prismClipWidget[i]->GetEnabled())
            {
                m_prismClipWidget[i]->On();
            }
            else
            {
                m_prismClipWidget[i]->Off();
            }
        }
    }
    qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::OnRenderOriginal()
{
    if (true)
    {
        if (SCENE_STATE_PLANE_CLIP == m_sceneManager.GetSceneState())
        {
            m_clipPlaneWidget->Off();
            m_clipPlaneWidget->EnabledOff();
        }

        if (SCENE_STATE_BOX_CLIP == m_sceneManager.GetSceneState())
        {
            m_boxClipWidget->Off();
            m_boxClipWidget->EnabledOff();
        }

        if (SCENE_STATE_PRISM_CLIP == m_sceneManager.GetSceneState())
        {
            for (int i = 0; i < 3; i++)
            {
                m_prismClipWidget[i]->Off();
                m_prismClipWidget[i]->EnabledOff();
            }
        }
        m_sceneManager.SetSceneState(SCENE_STATE_ORIGINAL);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_ORIGINAL);
    }
}

void MainWindow::OnQuickTurnCamLight()
{
    int camLightOn = m_camLight->GetSwitch();
    m_camLight->SetSwitch(!camLightOn);

    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnQuickTurnSceneLight()
{
    int sceneLightOn = m_sceneLightUp->GetSwitch();
    m_sceneLightUp->SetSwitch(!sceneLightOn);
    m_sceneLightDown->SetSwitch(!sceneLightOn);

    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnOrientationOnOff()
{
    int markerEnable = m_OrientationMarker->GetEnabled();
    m_OrientationMarker->SetEnabled(!markerEnable);
    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnTurnCubeAxesOnOff()
{
    int isOn = !(m_cubeAxesActor->GetVisibility());
    int xGridOn = m_cubeAxesActor->GetDrawXGridlines();
    int yGridOn = m_cubeAxesActor->GetDrawYGridlines();
    TurnCubeAxesOnOff(isOn, xGridOn, yGridOn);
}

void MainWindow::OnLightOption()
{
    double sceneLightPos[3];
    m_sceneLightUp->GetPosition(sceneLightPos);
    LightOption lightOptionDLG(m_camLight->GetSwitch(),
                               m_camLight->GetIntensity(),
                               m_sceneLightUp->GetSwitch()
                               &&m_sceneLightDown->GetSwitch(),
                               m_sceneLightUp->GetIntensity(),
                               sceneLightPos,
                               this);
    lightOptionDLG.setWindowModality(Qt::WindowModal);
    lightOptionDLG.exec();

    m_camLight->SetSwitch(lightOptionDLG.isCamLightOn);
    m_camLight->SetIntensity(lightOptionDLG.intencityCamLight);
    m_sceneLightUp->SetSwitch(lightOptionDLG.isSceneLightOn);
    m_sceneLightDown->SetSwitch(lightOptionDLG.isSceneLightOn);
    m_sceneLightUp->SetIntensity(lightOptionDLG.intencitySceneLight);
    m_sceneLightDown->SetIntensity(lightOptionDLG.intencitySceneLight);
    m_sceneLightUp->SetPosition(lightOptionDLG.xSceneLight,
                                lightOptionDLG.ySceneLight,
                                lightOptionDLG.zSceneLight);
    m_sceneLightDown->SetPosition(0 - lightOptionDLG.xSceneLight,
                                  0 - lightOptionDLG.ySceneLight,
                                  0 - lightOptionDLG.zSceneLight);
}

void MainWindow::OnCubeAxesOption()
{
    CubeAxesOption cubeAxesOptionDialog(m_cubeAxesActor->GetVisibility(),
                                        m_cubeAxesActor->GetDrawXGridlines(),
                                        m_cubeAxesActor->GetDrawYGridlines(),
                                        this);
    cubeAxesOptionDialog.setWindowModality(Qt::WindowModal);
    cubeAxesOptionDialog.exec();

    TurnCubeAxesOnOff(cubeAxesOptionDialog.m_isCubeAxesOn,
                      cubeAxesOptionDialog.m_isXGridOn,
                      cubeAxesOptionDialog.m_isYGridOn);
}

void MainWindow::OnStdExplode()
{
    if (m_sceneManager.GetSceneState() != SCENE_STATE_ORIGINAL)
    {
        QMessageBox::information(this,
                                 tr("提示"),
                                 tr("请在原始模型状态进行操作。"),
                                 QMessageBox::Yes);
        return;
    }
    StdExplode stdExplodeDLG;
    stdExplodeDLG.setWindowModality(Qt::WindowModal);
    stdExplodeDLG.exec();

    if (SCENE_STATE_ORIGINAL == m_sceneManager.GetSceneState())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_STD_EXPLODE);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        //cross explode
        if (1 == stdExplodeDLG.m_RadioCrossExplodeChecked)
        {
            m_evenExplodeRow = 2;
            m_evenExplodeCol = 2;
            m_evenExplodeGapRatio = 1;
            processRenderRequest(SCENE_STATE_STD_EXPLODE);
        } else if (1 == stdExplodeDLG.m_RadioEvenExplodeChecked)
        {
            m_evenExplodeRow = stdExplodeDLG.m_evenRow;
            m_evenExplodeCol = stdExplodeDLG.m_evenCol;
            m_evenExplodeGapRatio = stdExplodeDLG.m_gapRatio;
            processRenderRequest(SCENE_STATE_STD_EXPLODE);
        } else
        {
            m_sceneManager.SetSceneState(SCENE_STATE_ORIGINAL);
            processRenderRequest(SCENE_STATE_ORIGINAL);
        }
    }
}

void MainWindow::OnShowColorLegend()
{
    if (ui.dockColorLegend->isHidden())
    {
        ui.dockColorLegend->show();
    } else
    {
        ui.dockColorLegend->hide();
    }
}

void MainWindow::OnProjectExplorer()
{
    if (m_ProjectExplorer->isHidden())
    {
        m_ProjectExplorer->show();
    } else
    {
        m_ProjectExplorer->hide();
    }
}

void MainWindow::OnEditColorLegend()
{
    ColorLegendEditor* editor = new ColorLegendEditor(m_ColorLegendManager, this);
    editor->exec();
    delete editor;
}

void MainWindow::initPrjExplorer()
{
    QString prjFileName(m_pDoc->getProjectPathName().c_str());
    QFileInfo gvpFileInfo(prjFileName.toStdString().c_str());
    QString prjName(gvpFileInfo.baseName());
    QString header(tr("项目 \"") + prjName + tr("\""));
    m_prjTreeWidget->setHeaderLabel(header);

    initObjectItems();
    initFavItems();

}

void MainWindow::OnPrjExplorerTreeItemClicked(QTreeWidgetItem* item, int column)
{
    GVSPrjTreeWidgetItem* item_clicked = dynamic_cast<GVSPrjTreeWidgetItem *>(item);
    if (NULL == item_clicked)
    {
        return;
    }

    if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_OBJ)
    {
        prjExplorerObjItemClicked(item_clicked);
    }
    else if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_FAV_ITEM)
    {
        prjExplorerFavItemClicked(item_clicked);
    }
    else if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_FAV_GROUP)
    {
        prjExplorerFavGroupClicked(item_clicked);
    }
    else if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_MODEL)
    {
        prjExplorerModelClicked(item_clicked);
    }

//     if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_OBJ ||
//         item_clicked->getType() == PRJ_TREE_ITEM_TYPE_FAV_ITEM)
//     {
//         bool isMdlItemChckd = true;
//         for (int i = 0; i < item->parent()->childCount(); ++i)
//         {
//             Qt::CheckState childStateByIdx = item->parent()->child(i)->checkState(0);
//             bool isObjItemChckd = (childStateByIdx == Qt::Checked) ? true : false;
//             isMdlItemChckd = isMdlItemChckd && isObjItemChckd;
//         }
//         item->parent()->setCheckState(0, isMdlItemChckd ? Qt::Checked : Qt::Unchecked);
// 
//         //update obj and actor state.
//         bool objVisible = (item->checkState(0) == Qt::Checked) ? true : false;
//         if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_OBJ)
//         {
//             updateObjItem(item->parent()->text(0), item->text(0), objVisible);
//         }
//         else
//         {
//             throw std::exception("to be finished.");
//         }
//     }
//     else if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_MODEL ||
//              item_clicked->getType() == PRJ_TREE_ITEM_TYPE_FAV_GROUP)
//     {
//         for (int i = 0; i < item->childCount(); ++i)
//         {
//             if (item->checkState(0) != item->child(i)->checkState(0))
//             {
//                 item->child(i)->setCheckState(0, item->checkState(0));
// 
//                 Qt::CheckState childStateByIdx = item->child(i)->checkState(0);
//                 bool objVisible = (childStateByIdx == Qt::Checked) ? true : false;
//                 if (item_clicked->getType() == PRJ_TREE_ITEM_TYPE_MODEL)
//                 {
//                     updateObjItem(item->text(0), item->child(i)->text(0),objVisible);
//                 }
//                 else
//                 {
//                     throw std::exception("to be finished.");
//                 }
//             }
//         }
//     }
}

void MainWindow::prjExplorerObjItemClicked(GVSPrjTreeWidgetItem* item)
{
    if (item->getType() != PRJ_TREE_ITEM_TYPE_OBJ)
    {
        return;
    }

    //update obj and actor state.
    bool objVisible = (item->checkState(0) == Qt::Checked) ? true : false;
    updateObjItem(item->parent()->text(0), item->text(0), objVisible);

    //update model check state
    refreshModelCheckState(item->parent());

    //update favitem check state, here is not a good way in time complexity.
    ObjectManager* objManager = m_pDoc->GetObjManager();
    vector<FavGroup>* favTreeInDoc = objManager->getFavTree();
    vector<FavGroup>::iterator groupIterInDoc = favTreeInDoc->begin();
    for ( ; groupIterInDoc < favTreeInDoc->end(); groupIterInDoc++)
    {
        QString groupName(groupIterInDoc->getGroupName().c_str());
        vector<FavItem>::iterator favItemIterInView = groupIterInDoc->vecOfItems.begin();
        for ( ; favItemIterInView < groupIterInDoc->vecOfItems.end(); favItemIterInView++)
        {
            if (0 == favItemIterInView->getModelName().compare(item->parent()->text(0)) &&
                0 == favItemIterInView->getObjName().compare(item->text(0)))
            {
                QString favName(favItemIterInView->getName());

                QTreeWidgetItem* favItemInView = findFavItemInPrjTree(groupName, favName);
                if (NULL == favItemInView)
                {
                    return;
                }
                favItemInView->setCheckState(0, item->checkState(0));
                refreshGroupCheckState(favItemInView->parent());
            }
        }
    }

}

QTreeWidgetItem* MainWindow::findObjItemInPrjTree(QString modelName, QString objName)
{
    QList<QTreeWidgetItem*> modelList = m_prjTreeWidget->findItems(modelName,
                                                                   Qt::MatchCaseSensitive
                                                                   | Qt::MatchExactly);
    QTreeWidgetItem* modelItemInView = modelList[0];
    for (int i = 0; i < modelItemInView->childCount(); ++i)
    {
        if (modelItemInView->child(i)->text(0).compare(objName) == 0)
        {
            return modelItemInView->child(i);
        }
    }
    return NULL;
}

void MainWindow::prjExplorerFavItemClicked(GVSPrjTreeWidgetItem* item)
{
    if (item->getType() != PRJ_TREE_ITEM_TYPE_FAV_ITEM)
    {
        return;
    }

    //update group check state
    refreshGroupCheckState(item->parent());

    //find Fav record in doc
    ObjectManager* objManager = m_pDoc->GetObjManager();
    FavItem* favItemInDoc =
            objManager->findFavItemByName(item->parent()->text(0), item->text(0));
    if (NULL == favItemInDoc)
    {
        return;
    }

    //change vis of obj in doc
    QString modelName = favItemInDoc->getModelName();
    QString objName = favItemInDoc->getObjName();

    updateObjItem(modelName, objName, (item->checkState(0) == Qt::Checked) ? true : false);

    //change obj item check state in main window.
    QTreeWidgetItem* objItemInView = findObjItemInPrjTree(modelName, objName);
    if (NULL == objItemInView)
    {
        return;
    }
    objItemInView->setCheckState(0, item->checkState(0));

    //refresh model check state
    refreshModelCheckState(objItemInView->parent());
}

QTreeWidgetItem* MainWindow::findFavItemInPrjTree(QString groupName, QString favName)
{
    QList<QTreeWidgetItem*> tmpList = m_prjTreeWidget->findItems("Favorite",
                                                                 Qt::MatchCaseSensitive |
                                                                 Qt::MatchExactly);

    QTreeWidgetItem* favRoot = tmpList[0];
    for (int i = 0; i < favRoot->childCount(); ++i)
    {
        if (favRoot->child(i)->text(0).compare(groupName) == 0)
        {
            for (int j = 0; j < favRoot->child(i)->childCount(); ++j)
            {
                if (favRoot->child(i)->child(j)->text(0).compare(favName) == 0)
                {
                    return favRoot->child(i)->child(j);
                }
            }
        }
    }
    return NULL;
}

void MainWindow::refreshGroupCheckState(QTreeWidgetItem* groupWidget)
{
    GVSPrjTreeWidgetItem* groupItem = dynamic_cast<GVSPrjTreeWidgetItem*>(groupWidget);
    if (groupItem->getType() != PRJ_TREE_ITEM_TYPE_FAV_GROUP)
    {
        return;
    }

    bool isGroupItemChckd = true;
    for (int i = 0; i < groupItem->childCount(); ++i)
    {
        Qt::CheckState childStateByIdx = groupItem->child(i)->checkState(0);
        bool isFavItemChckd = (childStateByIdx == Qt::Checked) ? true : false;
        isGroupItemChckd = isGroupItemChckd && isFavItemChckd;
    }
    groupItem->setCheckState(0, isGroupItemChckd ? Qt::Checked : Qt::Unchecked);
}

void MainWindow::refreshModelCheckState(QTreeWidgetItem* modelWidget)
{
    GVSPrjTreeWidgetItem* modelItem = dynamic_cast<GVSPrjTreeWidgetItem*>(modelWidget);
    if (modelItem->getType() != PRJ_TREE_ITEM_TYPE_MODEL)
    {
        return;
    }

    bool isModelItemChckd = true;
    for (int i = 0; i < modelItem->childCount(); ++i)
    {
        Qt::CheckState childStateByIdx = modelItem->child(i)->checkState(0);
        bool isObjItemChckd = (childStateByIdx == Qt::Checked) ? true : false;
        isModelItemChckd = isModelItemChckd && isObjItemChckd;
    }
    modelItem->setCheckState(0, isModelItemChckd ? Qt::Checked : Qt::Unchecked);
}

void MainWindow::prjExplorerFavGroupClicked(GVSPrjTreeWidgetItem* item_clicked)
{
    throw std::exception("The method or operation is not implemented.");
}

void MainWindow::prjExplorerModelClicked(GVSPrjTreeWidgetItem* item_clicked)
{
    if (item_clicked->getType() != PRJ_TREE_ITEM_TYPE_MODEL)
    {
        return;
    }

    Qt::CheckState newModelState = item_clicked->checkState(0);

    for (int i = 0; i < item_clicked->childCount(); ++i)
    {
        item_clicked->child(i)->setCheckState(0, newModelState);
        GVSPrjTreeWidgetItem* objItemInView =
                dynamic_cast<GVSPrjTreeWidgetItem*>(item_clicked->child(i));
        prjExplorerObjItemClicked(objItemInView);
    }
}

void MainWindow::updateObjItem(QString modelName, QString objName, bool vis)
{
    if (m_pDoc->getObjVisByName(modelName, objName) == vis)
    {
        return;
    }
    if (!(m_pDoc->setObjVisByName(modelName, objName, vis)))
    {
        throw std::exception("Obj not found in object manager.");
    }

    if (m_sceneManager.GetSceneState() == SCENE_STATE_ORIGINAL)
    {
        QString actorName(modelName + "/" + objName);
        if (!(this->setActorVisByName(actorName, vis)))
        {
            throw std::exception("Actor not found in scene manager.");
        }

        //refresh
        if (qvtkWidget->GetRenderWindow())
        {
            qvtkWidget->GetRenderWindow()->Render();
        }
    }
}

bool MainWindow::setActorVisByName( QString actorName, bool vis )
{
    ActorRecord* record = m_sceneManager.getActorRecordByName(actorName.toStdString());
    if (record && record->actor)
    {
        record->isVisible = vis;
        record->actor->SetVisibility(vis);
        return true;
    }
    return false;
}

void MainWindow::initObjectItems()
{
    vector<Model>* objTree = m_pDoc->GetObjManager()->getObjTree();

    vector<Model>::iterator model_iter = objTree->begin();
    for ( ; model_iter != objTree->end(); model_iter++)
    {
        GVSPrjTreeWidgetItem* modelItem =
                new GVSPrjTreeWidgetItem(m_prjTreeWidget, PRJ_TREE_ITEM_TYPE_MODEL);
        modelItem->setText(0, model_iter->name);
        modelItem->setExpanded(true);
        modelItem->setFlags(Qt::ItemIsUserCheckable |
                            Qt::ItemIsEnabled |
                            Qt::ItemIsSelectable);
        bool isModelItemChecked = true;

        vector<GeoObject>::iterator obj_Iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_Iter != model_iter->vecOfGeoObjs.end(); obj_Iter++)
        {
            GVSPrjTreeWidgetItem* objItem =
                    new GVSPrjTreeWidgetItem(modelItem, PRJ_TREE_ITEM_TYPE_OBJ);
            objItem->setText(0, obj_Iter->getName());
            objItem->setFlags(Qt::ItemIsUserCheckable |
                              Qt::ItemIsEnabled |
                              Qt::ItemIsSelectable);

            bool isObjItemChecked = obj_Iter->getVisibility();
            objItem->setCheckState(0, isObjItemChecked ? Qt::Checked : Qt::Unchecked);

            QString iconFileName;
            switch (obj_Iter->getType())
            {
            case GEO_OBJECT_TYPE_POINT:
                iconFileName = tr(":/Resources/PrjExplorer/PointObj.png");
                break;
            case GEO_OBJECT_TYPE_LINE:
                iconFileName = tr(":/Resources/PrjExplorer/LineObj.png");
                break;
            case GEO_OBJECT_TYPE_SURFACE:
                iconFileName = tr(":/Resources/PrjExplorer/SurfaceObj.png");
                break;
            }
            objItem->setIcon(0, QIcon(iconFileName));

            isModelItemChecked = isModelItemChecked && isObjItemChecked;
        }
        modelItem->setCheckState(0, isModelItemChecked ? Qt::Checked : Qt::Unchecked);
    }

    connect(m_prjTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(OnPrjExplorerTreeItemClicked(QTreeWidgetItem*, int)));
}

void MainWindow::initFavItems()
{
    vector<FavGroup>* favTree = m_pDoc->GetObjManager()->getFavTree();

    GVSPrjTreeWidgetItem* favRootItem =
            new GVSPrjTreeWidgetItem(m_prjTreeWidget, PRJ_TREE_ITEM_TYPE_FAV_ROOT);
    favRootItem->setText(0, tr("Favorite"));
    favRootItem->setExpanded(true);

    vector<FavGroup>::iterator favGroup_iter = favTree->begin();
    for ( ; favGroup_iter != favTree->end(); favGroup_iter++)
    {
        GVSPrjTreeWidgetItem* group =
                new GVSPrjTreeWidgetItem(favRootItem, PRJ_TREE_ITEM_TYPE_FAV_GROUP);
        group->setText(0, tr(favGroup_iter->getGroupName().c_str()));
        group->setExpanded(true);
        group->setFlags(Qt::ItemIsEnabled |
                        Qt::ItemIsSelectable |
                        Qt::ItemIsUserCheckable);

        bool isGroupNodeChecked = (favGroup_iter->vecOfItems.size() > 0) ? true : false;

        vector<FavItem>::iterator favItem_Iter = favGroup_iter->vecOfItems.begin();
        for ( ; favItem_Iter != favGroup_iter->vecOfItems.end(); favItem_Iter++)
        {
            GVSPrjTreeWidgetItem* favItem =
                    new GVSPrjTreeWidgetItem(group, PRJ_TREE_ITEM_TYPE_FAV_ITEM);
            favItem->setText(0, favItem_Iter->getName());
            favItem->setFlags(Qt::ItemIsUserCheckable |
                              Qt::ItemIsEnabled |
                              Qt::ItemIsSelectable);

            ObjectManager* objManager = m_pDoc->GetObjManager();
            QString modelName = favItem_Iter->getModelName();
            QString objName = favItem_Iter->getObjName();
            GeoObject* obj = objManager->findObjByName(modelName, objName);

            if (obj)
            {
                QString iconFileName;
                switch (obj->getType())
                {
                case GEO_OBJECT_TYPE_POINT:
                    iconFileName = tr(":/Resources/PrjExplorer/PointObj.png");
                    break;
                case GEO_OBJECT_TYPE_LINE:
                    iconFileName = tr(":/Resources/PrjExplorer/LineObj.png");
                    break;
                case GEO_OBJECT_TYPE_SURFACE:
                    iconFileName = tr(":/Resources/PrjExplorer/SurfaceObj.png");
                    break;
                }
                favItem->setIcon(0, QIcon(iconFileName));

                bool isVis = obj->getVisibility();
                favItem->setCheckState(0, isVis ? Qt::Checked : Qt::Unchecked);

                isGroupNodeChecked = isGroupNodeChecked && isVis;
            }
            else
            {
                favItem->setIcon(0, QIcon(tr(":/Resources/PrjExplorer/NoObj.png")));
                favItem->setCheckState(0, Qt::Unchecked);
                isGroupNodeChecked = false;
            }
        }
        group->setCheckState(0, isGroupNodeChecked ? Qt::Checked : Qt::Unchecked);
    }
}

void MainWindow::OnChangingObjColor(QString& modelName, QString& objName)
{
    int oldRGB[3];
    if (m_pDoc->GetObjManager()->getObjColorByName(modelName, objName, oldRGB))
    {
        QColor color = QColorDialog::getColor(QColor(oldRGB[0], oldRGB[1], oldRGB[2]));
        if (color.isValid())
        {
            //if no color changed, just return
            if (color.red() == oldRGB[0] &&
                color.green() == oldRGB[1] &&
                color.blue() == oldRGB[2])
            {
                return;
            }

            bool setOk = m_pDoc->GetObjManager()->setObjColorByName(modelName, objName,
                                                                    color.red(),
                                                                    color.green(),
                                                                    color.blue());

            if (setOk && this->getQVTKWidget()->GetRenderWindow())
            {
                this->getQVTKWidget()->GetRenderWindow()->Render();
            }
            else if (!setOk)
            {
                QMessageBox::warning(NULL, tr("出错啦"), tr("无法设置对象颜色！"));
            }

        }
        else
        {
            QMessageBox::warning(NULL, tr("出错啦"), tr("设定颜色值非法！"));
        }
    }
    else
    {
        //QMessageBox::warning(NULL, tr("出错啦"), tr("无法得到对象原始颜色！"));
    }
}

void MainWindow::OnAddFavGroup()
{
    AddFavGroupDialog dlgAddFavGroup(this);
    dlgAddFavGroup.exec();
    if (dlgAddFavGroup.result() == QDialog::Accepted)
    {
        QString groupName(dlgAddFavGroup.getGroupName());

        if (NULL != m_pDoc->GetObjManager()->findFavGroupByName(groupName))
        {
            QMessageBox::information(NULL, tr("非法名称"), tr("已经存在该收藏夹。"));

            return;
        }

        if (false == (m_pDoc->GetObjManager()->addFavGroup(groupName)))
        {
            QMessageBox::information(NULL, tr("错误"), tr("无法添加收藏夹。"));
        }

        QList<QTreeWidgetItem*> itemList =
                m_prjTreeWidget->findItems("Favorite",
                                           Qt::MatchExactly |
                                           Qt::MatchCaseSensitive);

        if (itemList.size() != 1)
        {
            QMessageBox::information(NULL, tr("错误"), tr("Favorite节点出错。"));
            return;
        }
        GVSPrjTreeWidgetItem* favRootItem =
                dynamic_cast<GVSPrjTreeWidgetItem*>(itemList.first());

        GVSPrjTreeWidgetItem* newGroup =
                new GVSPrjTreeWidgetItem(favRootItem, PRJ_TREE_ITEM_TYPE_FAV_GROUP);
        newGroup->setText(0, groupName);
        newGroup->setExpanded(true);
        newGroup->setFlags(Qt::ItemIsEnabled |
                           Qt::ItemIsSelectable |
                           Qt::ItemIsUserCheckable);
        refreshGroupCheckState(newGroup);
    }

    return;
}

void MainWindow::OnAddFavItem(GVSPrjTreeWidgetItem& currTreeItem)
{
    if (currTreeItem.getType() == PRJ_TREE_ITEM_TYPE_FAV_GROUP ||
        currTreeItem.getType() == PRJ_TREE_ITEM_TYPE_FAV_ROOT)
    {
        ObjectManager* objManager = m_pDoc->GetObjManager();

        AddFavItemDialog addFavItemDlg(objManager);
        //prepare group combo box ctrl.
        const vector<FavGroup>* favTree = objManager->getFavTree();
        for (unsigned int i = 0; i < favTree->size(); ++i)
        {
            QString tmpGroupName(favTree->at(i).getGroupName().c_str()); 
            addFavItemDlg.getGroupComboBox()->addItem(tmpGroupName);

            if (currTreeItem.getType() == PRJ_TREE_ITEM_TYPE_FAV_GROUP)
            {
                if (0 == currTreeItem.text(0).compare(tmpGroupName))
                {
                    addFavItemDlg.getGroupComboBox()->setCurrentIndex(i);
                }
            }
        }
        //prepare model combo box ctrl.
        const vector<Model>* objTree = objManager->getObjTree();
        for (unsigned int i = 0; i < objTree->size(); ++i)
        {
            QString tmpModelName(objTree->at(i).name);
            addFavItemDlg.getModelComboBox()->addItem(tmpModelName);
        }
        //prepare obj combo box ctrl.
        QString currModelName = addFavItemDlg.getModelComboBox()->currentText();
        const Model* pModel = objManager->findModelByName(currModelName);
        if (NULL == pModel)
        {
            QMessageBox::information(NULL, tr("错误"), tr("模型名错误。"));
            return;
        }
        for (unsigned int i = 0; i < pModel->vecOfGeoObjs.size(); ++i)
        {
            QString tmpObjName(pModel->vecOfGeoObjs.at(i).getName());
            addFavItemDlg.getObjComboBox()->addItem(tmpObjName);
        }


        addFavItemDlg.exec();
        if (addFavItemDlg.result() == QDialog::Accepted)
        {
            QString newGroupName(addFavItemDlg.getGroupComboBox()->currentText());
            QString newFavItemName(addFavItemDlg.getFavItemLineEdit()->text());
            QString newModelName(addFavItemDlg.getModelComboBox()->currentText());
            QString newObjName(addFavItemDlg.getObjComboBox()->currentText());

            FavItem newFavItem(newFavItemName, newModelName, newObjName);
            if (!(objManager->addFavItem(newGroupName, newFavItem)))
            {
                QMessageBox::information(NULL, tr("错误"), tr("插入新记录失败。"));
            }

            //find the group node and add an item of tree.
            QList<QTreeWidgetItem*> itemList =
                    m_prjTreeWidget->findItems("Favorite",
                                               Qt::MatchExactly |
                                               Qt::MatchCaseSensitive);

            if (itemList.size() != 1)
            {
                QMessageBox::information(NULL, tr("错误"), tr("Favorite节点出错。"));
                return;
            }

            GVSPrjTreeWidgetItem* favRootItem =
                    dynamic_cast<GVSPrjTreeWidgetItem*>(itemList.first());

            for (int i = 0; i < favRootItem->childCount(); ++i)
            {
                QTreeWidgetItem* group = favRootItem->child(i);
                if (0 == newGroupName.compare(group->text(0)))
                {
                    GVSPrjTreeWidgetItem* newItem =
                            new GVSPrjTreeWidgetItem(group, PRJ_TREE_ITEM_TYPE_FAV_ITEM);
                    newItem->setText(0, newFavItemName);
                    newItem->setFlags(Qt::ItemIsUserCheckable |
                                      Qt::ItemIsEnabled |
                                      Qt::ItemIsSelectable);

                    GeoObject* obj = objManager->findObjByName(newModelName, newObjName);
                    if (obj)
                    {
                        QString iconFileName;
                        switch (obj->getType())
                        {
                        case GEO_OBJECT_TYPE_POINT:
                            iconFileName = tr(":/Resources/PrjExplorer/PointObj.png");
                            break;
                        case GEO_OBJECT_TYPE_LINE:
                            iconFileName = tr(":/Resources/PrjExplorer/LineObj.png");
                            break;
                        case GEO_OBJECT_TYPE_SURFACE:
                            iconFileName = tr(":/Resources/PrjExplorer/SurfaceObj.png");
                            break;
                        }
                        newItem->setIcon(0, QIcon(iconFileName));

                        bool isVis = obj->getVisibility();
                        newItem->setCheckState(0, isVis ? Qt::Checked : Qt::Unchecked);
                    }
                    else
                    {
                        newItem->setIcon(0, QIcon(tr(":/Resources/PrjExplorer/NoObj.png")));
                        newItem->setCheckState(0, Qt::Unchecked);
                    }
                }
                refreshGroupCheckState(group);
                break;
            }
        }

    }

    return;
}



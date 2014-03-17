#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTextEdit>
#include <QVBoxLayout>
#include <stdio.h>
#include <stdlib.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <Windows.h>
#include "MainWindow.h"
#include "GVSDoc.h"
#include "Options/LightOption.h"
#include <Options/CubeAxesOption.h>
#include <Options/StdExplode.h>

//#define GVS_SHOW_SPLISH

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    //show startup image while preparing.
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/Resources/startup.jpg"));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
#ifdef GVS_SHOW_SPLISH
    splash->showMessage(QObject::tr("对象初始化"), topRight,Qt::red);
    Sleep(1000);
#endif

    initMembers();
    bindingActionsWithSlots();

#ifdef GVS_SHOW_SPLISH
    splash->showMessage(QObject::tr("加载本地配置"), topRight, Qt::red);
    Sleep(1000);
#endif
    loadLocalConfig();
#ifdef GVS_SHOW_SPLISH
    splash->showMessage(QObject::tr("设置网络"), topRight, Qt::red);
    Sleep(1000);
#endif
    loadNetwork();
    splash->clearMessage();
    splash->close();
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

    statusBartipLabel = new QLabel(QObject::tr("ready"), statBar);
    statusBartipLabel->setAlignment(Qt::AlignHCenter);
    statusBartipLabel->setMinimumSize(statusBartipLabel->sizeHint());

    statusBartipLabel2 = new QLabel(QObject::tr("second tip"), statBar);
    statusBartipLabel2->setIndent(3);


    statBar->addWidget(statusBartipLabel);
    statBar->addWidget(statusBartipLabel2);
}

void MainWindow::initMainAreaMembers()
{
    splitterMain = new QSplitter(Qt::Horizontal);
    splitterMain->setParent(this);

    //left init
    leftTab = new QTabWidget(splitterMain);
    treeTab1 = new QTreeView(leftTab);
    treeTab2 = new QTreeView(leftTab);
    leftTab->addTab(treeTab1, tr("视图1"));
    leftTab->addTab(treeTab2, tr("视图2"));


    //right side init
    qvtkWidget = new QVTKWidget(splitterMain);

    splitterMain->setStretchFactor(1, 1);
    setCentralWidget(splitterMain);
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
    m_sceneLightUp->SetPosition(1,1,1);
    m_mainRenderer->AddLight(m_sceneLightUp);
    m_sceneLightDown = vtkOpenGLLight::New();
    m_sceneLightDown->SetLightTypeToSceneLight();
    m_sceneLightDown->SetSwitch(1);
    m_sceneLightDown->SetIntensity(1);
    m_sceneLightDown->SetPosition(-1,-1,-1);
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
}

void MainWindow::destoryStatusBarMembers()
{
    delete statusBartipLabel;
    delete statusBartipLabel2;
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
    for (int i=0;i<6;i++)
    {
        DeleteVTKPointer(m_boxClipPlane[i]);
    }

    for (int i=0;i<3;i++)
    {
        DeleteVTKPointer(m_prismClipPlane[i]);
        DeleteVTKPointer(m_prismClipWidget[i]);
        DeleteVTKPointer(m_prismClipRep[i]);
        DeleteVTKPointer(m_prismClipCallback[i]);
    }

    removeAllActorsOfRenderer(m_mainRenderer);
    m_sceneManager.ClearActorTable();

    DeleteVTKPointer(m_mainRenderer);

    delete(m_pDoc);
}

void MainWindow::welcomeYou()
{
    vtkRenderer* renderer = this->m_mainRenderer;
    qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    renderer->SetBackground(.32, .34, .43);
    qvtkWidget->GetRenderWindow()->Render();
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
    m_mainRenderer->Render();
    //////////////////////
    if (m_pDoc->GetObjectsManager()->GetObjectsTable()->size())
    {
        m_sceneManager.SetSceneState(SCENE_STATE_ORIGINAL);
        showOrientationMarker();
        processRenderRequest(m_sceneManager.GetSceneState());
    }
    if (qvtkWidget->GetRenderWindow()) {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::showOrientationMarker()
{
    vtkSmartPointer<vtkAxesActor> axesActor=vtkSmartPointer<vtkAxesActor>::New();
    axesActor->SetYAxisLabelText("N");
    axesActor->SetXAxisLabelText("E");
    axesActor->SetZAxisLabelText("UP");
    axesActor->GetXAxisShaftProperty()->SetColor(0,0,1);
    axesActor->GetYAxisShaftProperty()->SetColor(0,1,0);
    axesActor->GetZAxisShaftProperty()->SetColor(1,0,0);
    axesActor->GetXAxisTipProperty()->SetColor(0,0,1);
    axesActor->GetYAxisTipProperty()->SetColor(0,1,0);
    axesActor->GetZAxisTipProperty()->SetColor(1,0,0);
    axesActor->SetTotalLength(1,1,1);

    double backColor[3];
    m_mainRenderer->GetBackground(backColor);
    m_OrientationMarker->SetOutlineColor(backColor[0],backColor[1],backColor[2]);
    m_OrientationMarker->SetOrientationMarker( axesActor );
    m_OrientationMarker->SetInteractor( this->qvtkWidget->GetInteractor());
    m_OrientationMarker->SetOutlineColor(1,1,1);
    m_OrientationMarker->SetEnabled(1);
    m_OrientationMarker->SetViewport( 0.0, 1-0.2, 0.2, 1 );
    m_OrientationMarker->KeyPressActivationOff();
    m_OrientationMarker->InteractiveOn();
}

void MainWindow::removeAllActorsOfRenderer( vtkRenderer* renderer )
{
    renderer->RemoveAllViewProps();
}

void MainWindow::processRenderRequest( int state )
{
    removeAllActorsOfRenderer(m_mainRenderer);
    m_sceneManager.ClearActorTable();
    GVSDoc* pDoc = getDocument();
    if (!pDoc)
        return;
    else
    {
        if (pDoc->GetObjectsManager()->GetObjectsTable()->size()!=0)
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
    if (getDocument()->GetObjectsManager()->GetObjectsTable()->size()!=0)
    {
        m_sceneManager.SetSceneBounds(getDocument()->GetObjectsManager()->m_bounds);
        vector<ObjectRecord>::iterator iter_ObRcd=
                getDocument()->GetObjectsManager()->GetObjectsTable()->begin();
        for (; iter_ObRcd!=getDocument()->GetObjectsManager()->GetObjectsTable()->end(); iter_ObRcd++)
        {
            vtkActor* tempActor=MappingDataSetToActor(iter_ObRcd->reader->GetOutput());
            m_sceneManager.InsertActorRecord(tempActor,
                                             iter_ObRcd->fileName,
                                             SCENE_STATE_ORIGINAL,
                                             1);
        }
        m_sceneManager.AddCrrtStatActrToRnder(m_mainRenderer);
        m_mainRenderer->ResetCamera();
        m_mainRenderer->Render();
    }
}

void MainWindow::RenderPlaneClip()
{
    if (getDocument()->GetObjectsManager()->GetObjectsTable()->size()!=0)
    {
        vtkSmartPointer<vtkAppendFilter> apdFilter=
                vtkSmartPointer<vtkAppendFilter>::New();
        vector<ObjectRecord>::iterator iter_ObRcd=
                getDocument()->GetObjectsManager()->GetObjectsTable()->begin();
        for ( ; iter_ObRcd!=getDocument()->GetObjectsManager()->GetObjectsTable()->end(); iter_ObRcd++)
        {
            if(iter_ObRcd->visible)
            {
                iter_ObRcd->reader->Update();
                apdFilter->AddInput(iter_ObRcd->reader->GetOutput());
            }
        }
        apdFilter->Update();
        vtkDataSet* mergedDataSet=apdFilter->GetOutput();

        m_clipPlane->SetNormal(1, 0, 0);
        vtkSmartPointer<vtkTableBasedClipDataSet> clipper =
                vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        clipper->SetClipFunction(m_clipPlane);
        clipper->InsideOutOn();
        clipper->SetInput(mergedDataSet);
        clipper->Update();

        vtkActor* clipActor = MappingDataSetToActor(clipper->GetOutput());
        m_sceneManager.InsertActorRecord(clipActor,"_TempClipResult", SCENE_STATE_PLANE_CLIP, 1);
        m_sceneManager.AddCrrtStatActrToRnder(m_mainRenderer);

        m_clipPlaneRep->SetPlaceFactor(1.25);
        double tmpBounds[6];
        mergedDataSet->GetBounds(tmpBounds);
        m_clipPlaneRep->PlaceWidget(tmpBounds);
        m_clipPlaneRep->SetNormal(m_clipPlane->GetNormal());
        m_clipPlaneRep->SetOrigin((tmpBounds[0]+tmpBounds[1])/2, (tmpBounds[2]+tmpBounds[3])/2, (tmpBounds[5]+tmpBounds[4])/2);
        m_clipPlaneRep->DrawPlaneOff();

        m_clipCallback->Plane=m_clipPlane;
        m_clipCallback->Actor=clipActor;

        m_clipPlaneWidget->SetInteractor(this->qvtkWidget->GetInteractor());
        m_clipPlaneWidget->SetRepresentation(m_clipPlaneRep);
        m_clipPlaneWidget->AddObserver(vtkCommand::InteractionEvent, m_clipCallback);
        m_clipPlaneWidget->On();
        m_clipPlaneWidget->EnabledOn();
        //reset camera
        m_mainRenderer->ResetCamera();
        m_mainRenderer->Render();
    }
}

void MainWindow::RenderStdExplode()
{
    //append object data which visible=1.
    vtkSmartPointer<vtkAppendFilter> apdFilter=vtkSmartPointer<vtkAppendFilter>::New();
    vector<ObjectRecord>::iterator iter_ObRcd=
            getDocument()->GetObjectsManager()->GetObjectsTable()->begin();
    for ( ; iter_ObRcd!=getDocument()->GetObjectsManager()->GetObjectsTable()->end(); iter_ObRcd++)
    {
        if(iter_ObRcd->visible)
        {
            iter_ObRcd->reader->Update();
            apdFilter->AddInput(iter_ObRcd->reader->GetOutput());
        }
    }
    apdFilter->Update();
    vtkDataSet* mergedDataSet=apdFilter->GetOutput();
    //bounds
    double bounds[6];
    mergedDataSet->GetBounds(bounds);
    //gap size
    double explodeGap=m_evenExplodeGapRatio*(bounds[5]-bounds[4]);

    //--------------------------------horizon cut-------------------------------
    //temp var to store clipper, trans,etc.
    vtkSmartPointer<vtkDataSet> hResultData[evenRowLMT];
    vtkSmartPointer<vtkPlane> hPlane[evenRowLMT-1];
    vtkSmartPointer<vtkTableBasedClipDataSet> hClipper[evenRowLMT-1][2];
    vtkSmartPointer<vtkTransform> hTrans[evenRowLMT-1];
    vtkSmartPointer<vtkTransformFilter> hTransFilter[evenRowLMT-1];
    //temp var to be process in next loop.
    vtkSmartPointer<vtkDataSet> inputData=mergedDataSet;
    // horizon cut begin
    for (int i=0;i<m_evenExplodeRow-1;i++)
    {
        hPlane[i]=vtkSmartPointer<vtkPlane>::New();
        hPlane[i]->SetNormal(0, 1, 0);
        hPlane[i]->SetOrigin(0, (bounds[3]-(i+1)*(bounds[3]-bounds[2])/m_evenExplodeRow) , 0);
        //clipper positive, insideOutOff
        hClipper[i][0]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        hClipper[i][0]->SetInput(inputData);
        hClipper[i][0]->SetClipFunction(hPlane[i]);
        hClipper[i][0]->InsideOutOff();
        hClipper[i][0]->Update();
        //transFilter
        hTrans[i]=vtkSmartPointer<vtkTransform>::New();
        hTrans[i]->Translate(0, (m_evenExplodeRow-i-1)*explodeGap, 0);
        hTransFilter[i]=vtkSmartPointer<vtkTransformFilter>::New();
        hTransFilter[i]->SetInputConnection(hClipper[i][0]->GetOutputPort());
        hTransFilter[i]->SetTransform(hTrans[i]);
        hTransFilter[i]->Update();
        //store into tmp var
        hResultData[i]=hTransFilter[i]->GetOutput();
        //clipper nagtive, insideOutOn
        hClipper[i][1]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        hClipper[i][1]->SetInput(inputData);
        hClipper[i][1]->SetClipFunction(hPlane[i]);
        hClipper[i][1]->InsideOutOn();
        hClipper[i][1]->Update();
        //prepare for next loop
        inputData=hClipper[i][1]->GetOutput();
    }
    //store the last ClippedOutput in to hResultData.
    hResultData[m_evenExplodeRow-1]=inputData;

    //----------------------------------------vertical cut--------------------------
    //temp var to store clipper, trans,etc.
    vtkSmartPointer<vtkPlane> vPlane[evenRowLMT][evenColLMT-1];
    vtkSmartPointer<vtkTableBasedClipDataSet> vClipper[evenRowLMT][evenColLMT-1][2];
    vtkSmartPointer<vtkTransform> vTrans[evenRowLMT][evenColLMT-1];
    vtkSmartPointer<vtkTransformFilter> vTransFilter[evenRowLMT][evenColLMT-1];
    vtkSmartPointer<vtkDataSet> resultData[evenRowLMT][evenColLMT];//final result dataset array.
    //vertical cut begin
    for (int i=0;i<m_evenExplodeRow;i++)
    {
        inputData=hResultData[i];//for every horizon result data.
        for (int j=0;j<m_evenExplodeCol-1;j++)
        {
            vPlane[i][j]=vtkSmartPointer<vtkPlane>::New();
            vPlane[i][j]->SetNormal(1, 0, 0);
            vPlane[i][j]->SetOrigin( (bounds[1]-(j+1)*(bounds[1]-bounds[0])/m_evenExplodeCol) , 0, 0 );
            //clipper positive insideOutOff
            vClipper[i][j][0]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            vClipper[i][j][0]->SetInput(inputData);
            vClipper[i][j][0]->SetClipFunction(vPlane[i][j]);
            vClipper[i][j][0]->InsideOutOff();
            vClipper[i][j][0]->Update();
            //transFilter
            vTrans[i][j]=vtkSmartPointer<vtkTransform>::New();
            vTrans[i][j]->Translate( (m_evenExplodeCol-j-1)*explodeGap , 0, 0);
            vTransFilter[i][j]=vtkSmartPointer<vtkTransformFilter>::New();
            vTransFilter[i][j]->SetInputConnection(vClipper[i][j][0]->GetOutputPort());
            vTransFilter[i][j]->SetTransform(vTrans[i][j]);
            vTransFilter[i][j]->Update();
            //store into final data array.
            resultData[i][j]=vTransFilter[i][j]->GetOutput();
            //clipper nagtive, insideOutOn
            vClipper[i][j][1]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            vClipper[i][j][1]->SetInput(inputData);
            vClipper[i][j][1]->SetClipFunction(vPlane[i][j]);
            vClipper[i][j][1]->InsideOutOn();
            vClipper[i][j][1]->Update();
            //for next loop
            inputData=vClipper[i][j][1]->GetOutput();
        }
        resultData[i][m_evenExplodeCol-1]=inputData;
    }

    //mapping data to actor
    for (int i=0; i<m_evenExplodeRow; i++)
    {
        for (int j=0; j<m_evenExplodeCol; j++)
        {
            vtkActor* tmpActor=MappingDataSetToActor(resultData[i][j]);
            m_sceneManager.InsertActorRecord(tmpActor,"_StdExplodeResult",SCENE_STATE_STD_EXPLODE,true);
        }
    }
    //add all actor which state=stdExplode
    m_sceneManager.AddActorsByState(m_mainRenderer,SCENE_STATE_STD_EXPLODE);
    //render it.
    m_mainRenderer->ResetCamera();
    m_mainRenderer->Render();
}

void MainWindow::RenderPrismClip()
{
    if (getDocument()->GetObjectsManager()->GetObjectsTable()->size()!=0)
    {
        //append the object data which visible=1.
        vtkSmartPointer<vtkAppendFilter> source=
                vtkSmartPointer<vtkAppendFilter>::New();
        vector<ObjectRecord>::iterator iter_ObRcd=
                getDocument()->GetObjectsManager()->GetObjectsTable()->begin();
        for ( ; iter_ObRcd!=getDocument()->GetObjectsManager()->GetObjectsTable()->end(); iter_ObRcd++)
        {
            if(iter_ObRcd->visible)
            {
                iter_ObRcd->reader->Update();
                source->AddInput(iter_ObRcd->reader->GetOutput());
            }
        }
        source->Update();
        vtkSmartPointer<vtkAppendFilter> apdResult=
            vtkSmartPointer<vtkAppendFilter>::New();
        vtkDataSet* dataToClip=source->GetOutput();//appended result
        double bounds[6];
        dataToClip->GetBounds(bounds);
        for (int i=0;i<5;i++)
        {
            bounds[i]*=1.1;
        }
        m_prismClipPlane[0]->SetOrigin(source->GetOutput()->GetCenter());
        m_prismClipPlane[0]->SetNormal(0,0,1);
        vtkSmartPointer<vtkTableBasedClipDataSet> clipper11=
                vtkSmartPointer<vtkTableBasedClipDataSet>::New();
        clipper11->SetClipFunction(m_prismClipPlane[0]);
        clipper11->SetInput(dataToClip);
        clipper11->InsideOutOn();
        clipper11->Update();
        apdResult->AddInput(clipper11->GetOutput());
        vtkTableBasedClipDataSet* clipper12=vtkTableBasedClipDataSet::New();
        clipper12->SetClipFunction(m_prismClipPlane[0]);
        clipper12->SetInput(dataToClip);
        clipper12->InsideOutOff();
        clipper12->Update();
        dataToClip=clipper12->GetOutput();


        m_prismClipPlane[1]->SetOrigin(source->GetOutput()->GetCenter());
        m_prismClipPlane[1]->SetNormal(1,-1,0);
        vtkTableBasedClipDataSet* clipper21=vtkTableBasedClipDataSet::New();
        clipper21->SetClipFunction(m_prismClipPlane[1]);
        clipper21->SetInput(dataToClip);
        clipper21->InsideOutOn();
        clipper21->Update();
        apdResult->AddInput(clipper21->GetOutput());

        vtkTableBasedClipDataSet* clipper22=vtkTableBasedClipDataSet::New();
        clipper22->SetClipFunction(m_prismClipPlane[1]);
        clipper22->SetInput(dataToClip);
        clipper22->InsideOutOff();
        clipper22->Update();
        dataToClip=clipper22->GetOutput();


        m_prismClipPlane[2]->SetOrigin(source->GetOutput()->GetCenter());
        m_prismClipPlane[2]->SetNormal(-1,-1,0);
        vtkTableBasedClipDataSet* clipper31=vtkTableBasedClipDataSet::New();
        clipper31->SetClipFunction(m_prismClipPlane[2]);
        clipper31->SetInput(dataToClip);
        clipper31->InsideOutOn();
        clipper31->Update();
        apdResult->AddInput(clipper31->GetOutput());
        apdResult->Update();

        vtkActor* tmpActor=MappingDataSetToActor(apdResult->GetOutput() );
        m_sceneManager.InsertActorRecord(tmpActor,"_PrismClipResult", SCENE_STATE_PRISM_CLIP, 1);
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
        //m_prismClipCallback[1]=vtkPlaneClipCallback::New();
        m_prismClipCallback[1]->Plane=m_prismClipPlane[1];
        //widget2
        //m_prismClipWidget[1]=vtkImplicitPlaneWidget2::New();
        m_prismClipWidget[1]->SetInteractor(qvtkWidget->GetInteractor());
        m_prismClipWidget[1]->SetRepresentation(m_prismClipRep[1]);
        m_prismClipWidget[1]->AddObserver(vtkCommand::EndInteractionEvent, m_prismClipCallback[1]);
        m_prismClipWidget[1]->On();
        m_prismClipWidget[1]->EnabledOn();
        //----------------------------clip3------------------------------
        //rep3
        //m_prismClipRep[2]=vtkImplicitPlaneRepresentation::New();
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
        //m_prismClipCallback[2]=vtkPlaneClipCallback::New();
        m_prismClipCallback[2]->Plane=m_prismClipPlane[2];
        //widget2
        //m_prismClipWidget[2]=vtkImplicitPlaneWidget2::New();
        m_prismClipWidget[2]->SetInteractor(qvtkWidget->GetInteractor());
        m_prismClipWidget[2]->SetRepresentation(m_prismClipRep[2]);
        m_prismClipWidget[2]->AddObserver(vtkCommand::EndInteractionEvent, m_prismClipCallback[2]);
        m_prismClipWidget[2]->On();
        m_prismClipWidget[2]->EnabledOn();
        //----------------------------clip1------------------------------
        //m_prismClipRep[0]=vtkImplicitPlaneRepresentation::New();
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

        //m_prismClipCallback[0]=vtkPlaneClipCallback::New();
        m_prismClipCallback[0]->Plane=m_prismClipPlane[0];

        //m_prismClipWidget[0]=vtkImplicitPlaneWidget2::New();
        m_prismClipWidget[0]->SetInteractor(qvtkWidget->GetInteractor());
        m_prismClipWidget[0]->SetRepresentation(m_prismClipRep[0]);
        m_prismClipWidget[0]->AddObserver(vtkCommand::EndInteractionEvent, m_prismClipCallback[0]);
        m_prismClipWidget[0]->On();
        m_prismClipWidget[0]->EnabledOn();

        m_mainRenderer->ResetCamera();
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::RenderBoxClip()
{
    if (getDocument()->GetObjectsManager()->GetObjectsTable()->size()!=0)
    {
        vtkSmartPointer<vtkAppendFilter> apdOriginFilter=
                vtkSmartPointer<vtkAppendFilter>::New();
        vector<ObjectRecord>::iterator iter_ObRcd=
                getDocument()->GetObjectsManager()->GetObjectsTable()->begin();
        for ( ; iter_ObRcd!=getDocument()->GetObjectsManager()->GetObjectsTable()->end(); iter_ObRcd++)
        {
            if(iter_ObRcd->visible)
            {
                iter_ObRcd->reader->Update();
                apdOriginFilter->AddInput(iter_ObRcd->reader->GetOutput());
            }
        }
        apdOriginFilter->Update();
        double bounds[6];
        apdOriginFilter->GetOutput()->GetBounds(bounds);

        //box widget
        //m_boxClipWidget=vtkBoxWidget::New();
        m_boxClipWidget->SetInteractor(qvtkWidget->GetInteractor());
        m_boxClipWidget->SetPlaceFactor(1);
        m_boxClipWidget->SetInput(apdOriginFilter->GetOutput());
        m_boxClipWidget->PlaceWidget(  bounds[0],
            (bounds[0]+ (bounds[1]-bounds[0])/4),
            bounds[2],
            (bounds[2] + (bounds[3]-bounds[2])/4),
            (bounds[4]+bounds[5])/2,
            (bounds[5] + (bounds[5]-bounds[4])/4)
            );

        m_boxClipWidget->InsideOutOn();
        m_boxClipWidget->GetOutlineProperty()->SetRepresentationToWireframe();
        m_boxClipWidget->SetHandleSize(0.002);

        //planes
        //m_boxClipPlanes=vtkPlanes::New();
        m_boxClipWidget->GetPlanes(m_boxClipPlanes);
        for (int i=0; i<6;i++)
        {
            //m_boxClipPlane[i]=vtkPlane::New();
            m_boxClipPlanes->GetPlane(i, m_boxClipPlane[i]);
        }
        //box clip result
        vtkSmartPointer<vtkDataSet> result[6];
        vtkSmartPointer<vtkTableBasedClipDataSet> clipper[6][2];
        vtkDataSet* dataToClip=apdOriginFilter->GetOutput();
        //clip with 6 planes
        for (int i=0; i<6; i++)
        {
            clipper[i][0]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            clipper[i][0]->SetInput(dataToClip);
            clipper[i][0]->SetClipFunction(m_boxClipPlane[i]);
            clipper[i][0]->InsideOutOn();
            clipper[i][0]->Update();
            result[i]=clipper[i][0]->GetOutput();

            clipper[i][1]=vtkSmartPointer<vtkTableBasedClipDataSet>::New();
            clipper[i][1]->SetInput(dataToClip);
            clipper[i][1]->SetClipFunction(m_boxClipPlane[i]);
            clipper[i][1]->InsideOutOff();
            clipper[i][1]->Update();
            dataToClip=clipper[i][1]->GetOutput();
        }
        //append result
        vtkSmartPointer<vtkAppendFilter> apdResult=vtkSmartPointer<vtkAppendFilter>::New();
        for (int i=0; i<6; i++)
        {
            apdResult->AddInput(result[i]);
        }
        apdResult->Update();
        //callback
        //m_boxClipCallback=vtkBoxClipCallback::New();
        m_boxClipCallback->boxPlaneArray=m_boxClipPlane;
        m_boxClipCallback->boxWidget=m_boxClipWidget;
        m_boxClipWidget->AddObserver(vtkCommand::EndInteractionEvent, m_boxClipCallback);
        //insert record
        vtkActor* tmpActor=MappingDataSetToActor(apdResult->GetOutput());
        m_sceneManager.InsertActorRecord(tmpActor,"_BoxClipActor",SCENE_STATE_BOX_CLIP,1);
        //add actor to renderer
        m_mainRenderer->AddActor(tmpActor);
        //widget on
        m_boxClipWidget->On();
        m_boxClipWidget->EnabledOn();
    }
}

vtkActor* MainWindow::MappingDataSetToActor( vtkDataSet* ds )
{
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInput(ds);
    mapper->Update();
    vtkActor* actor=vtkActor::New();
    actor->SetMapper(mapper);
    return actor;
}

void MainWindow::TurnCubeAxesOnOff( int isOn, int xGridOn, int yGridOn )
{
    if (getDocument()->GetObjectsManager()->GetObjectsTable()->size()!=0)
    {
        if (m_sceneManager.GetSceneState()==SCENE_STATE_ORIGINAL)
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
}

void MainWindow::bindingActionsWithSlots()
{
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(OnOpenProject()));
    connect(ui.actionClipModel, SIGNAL(triggered()), this, SLOT(OnRenderClip()));
    connect(ui.actionCrossExplode, SIGNAL(triggered()), this, SLOT(OnCrossExplode()));
    connect(ui.actionCubeNotch, SIGNAL(triggered()), this, SLOT(OnRenderBoxClip()));
    connect(ui.actionShowHideBoxClipper, SIGNAL(triggered()),
            this, SLOT(OnBoxClipWidgetOnOff()));
    connect(ui.actionTriangleNotch, SIGNAL(triggered()), this, SLOT(OnPrismClip()));
    connect(ui.actionShowHideTriClipper, SIGNAL(triggered()), this,
                                                SLOT(OnPrismClipWidgetOnOff()));
    connect(ui.actionRenderOriginal, SIGNAL(triggered()), this, SLOT(OnRenderOriginal()));
    connect(ui.actionCamLight, SIGNAL(triggered()), this, SLOT(OnQuickTurnCamLight()));
    connect(ui.actionSceneLight, SIGNAL(triggered()), this, SLOT(OnQuickTurnSceneLight()));
    connect(ui.actionOrientatorCtrl, SIGNAL(triggered()), this, SLOT(OnOrientationOnOff()));
    connect(ui.actionRulerCtrl, SIGNAL(triggered()), this, SLOT(OnTurnCubeAxesOnOff()));
    connect(ui.actionRulerGridCtrl, SIGNAL(triggered()), this, SLOT(OnCubeAxesOption()));
    connect(ui.actionLightOption, SIGNAL(triggered()), this, SLOT(OnLightOption()));
    connect(ui.actionStdExplode, SIGNAL(triggered()), this, SLOT(OnStdExplode()));
    connect(ui.actionShowColorLegend, SIGNAL(triggered()), this, SLOT(OnColorLegend()));
}

void MainWindow::OnOpenProject()
{
    m_pDoc->OnOpenProject();

    onInitialUpdate();
}

void MainWindow::OnRenderClip()
{
    if (m_sceneManager.GetSceneState() == SCENE_STATE_ORIGINAL)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_PLANE_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_PLANE_CLIP);
    }
}

void MainWindow::OnCrossExplode()
{
    if (m_sceneManager.GetSceneState()==SCENE_STATE_ORIGINAL)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_STD_EXPLODE);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();

        m_evenExplodeRow=2;
        m_evenExplodeCol=2;
        m_evenExplodeGapRatio=1;
        processRenderRequest(SCENE_STATE_STD_EXPLODE);
    }
}

void MainWindow::OnRenderBoxClip()
{
    if (m_sceneManager.GetSceneState()==SCENE_STATE_ORIGINAL)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_BOX_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_BOX_CLIP);
    }
}

void MainWindow::OnBoxClipWidgetOnOff()
{
    if (m_sceneManager.GetSceneState()==SCENE_STATE_BOX_CLIP)
    {
        int isEnable=m_boxClipWidget->GetEnabled();
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
    if (m_sceneManager.GetSceneState()==SCENE_STATE_ORIGINAL)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_PRISM_CLIP);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        processRenderRequest(SCENE_STATE_PRISM_CLIP);
    }
}

void MainWindow::OnPrismClipWidgetOnOff()
{
    if (m_sceneManager.GetSceneState()==SCENE_STATE_PRISM_CLIP)
    {
        int isWidgetsOn=m_prismClipWidget[0]->GetEnabled();
        for (int i=0;i<3;i++)
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
        if (SCENE_STATE_PLANE_CLIP==m_sceneManager.GetSceneState())
        {
            m_clipPlaneWidget->Off();
            m_clipPlaneWidget->EnabledOff();
        }

        if (SCENE_STATE_BOX_CLIP==m_sceneManager.GetSceneState())
        {
            m_boxClipWidget->Off();
            m_boxClipWidget->EnabledOff();
        }

        if (SCENE_STATE_PRISM_CLIP==m_sceneManager.GetSceneState())
        {
            for (int i=0; i<3; i++)
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
    int camLightOn=m_camLight->GetSwitch();
    m_camLight->SetSwitch(!camLightOn);

    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnQuickTurnSceneLight()
{
    int sceneLightOn=m_sceneLightUp->GetSwitch();
    m_sceneLightUp->SetSwitch(!sceneLightOn);
    m_sceneLightDown->SetSwitch(!sceneLightOn);

    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnOrientationOnOff()
{
    int markerEnable=m_OrientationMarker->GetEnabled();
    m_OrientationMarker->SetEnabled(!markerEnable);
    if (qvtkWidget->GetRenderWindow())
    {
        qvtkWidget->GetRenderWindow()->Render();
    }
}

void MainWindow::OnTurnCubeAxesOnOff()
{
    int isOn=!m_cubeAxesActor->GetVisibility();
    int xGridOn=m_cubeAxesActor->GetDrawXGridlines();
    int yGridOn=m_cubeAxesActor->GetDrawYGridlines();
    TurnCubeAxesOnOff(isOn,xGridOn, yGridOn);
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
    m_sceneLightDown->SetPosition(0-lightOptionDLG.xSceneLight,
                                  0-lightOptionDLG.ySceneLight,
                                  0-lightOptionDLG.zSceneLight);
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

    if (m_sceneManager.GetSceneState()==SCENE_STATE_ORIGINAL)
    {
        m_sceneManager.SetSceneState(SCENE_STATE_STD_EXPLODE);
        m_mainRenderer->RemoveAllViewProps();
        m_sceneManager.ClearActorTable();
        //cross explode
        if (stdExplodeDLG.m_RadioCrossExplodeChecked==1)
        {
            m_evenExplodeRow=2;
            m_evenExplodeCol=2;
            m_evenExplodeGapRatio=1;
            processRenderRequest(SCENE_STATE_STD_EXPLODE);
        } else if (stdExplodeDLG.m_RadioEvenExplodeChecked==1)
        {
            m_evenExplodeRow=stdExplodeDLG.m_evenRow;
            m_evenExplodeCol=stdExplodeDLG.m_evenCol;
            m_evenExplodeGapRatio=stdExplodeDLG.m_gapRatio;
            processRenderRequest(SCENE_STATE_STD_EXPLODE);
        } else {
            m_sceneManager.SetSceneState(SCENE_STATE_ORIGINAL);
            processRenderRequest(SCENE_STATE_ORIGINAL);
        }
    }
}

void MainWindow::OnColorLegend()
{
    if (ui.dockColorLegend->isHidden())
    {
        ui.dockColorLegend->show();
    } else {
        ui.dockColorLegend->hide();
    }
}


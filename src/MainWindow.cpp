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
}

void MainWindow::destoryMembers()
{
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

void MainWindow::displayCube()
{
    vtkCubeSource* cubeSource = vtkCubeSource::New();
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(cubeSource->GetOutputPort());
    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    vtkRenderer* renderer = vtkRenderer::New();    
    vtkRenderWindow* renderWindow = qvtkWidget->GetRenderWindow();
    renderWindow->AddRenderer(renderer);
    renderer->AddActor(actor);
    renderer->SetBackground(.3, .2, .1);
    renderWindow->Render();
}


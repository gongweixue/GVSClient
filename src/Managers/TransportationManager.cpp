#include "TransportationManager.h"
#include "MainWindow.h"
#include "QMessageBox"
#include "Dialog/ConnectServerDialog.h"
#include "Records/ConnectionRecord.h"

TransportationManager::TransportationManager(MainWindow* window, QObject *parent)
    : QObject(parent)
{
    this->mainWindow = window;
    ftpConn = NULL;
    connList.clear();
    connectSignalSlots();
    initConnectionList();
}

TransportationManager::~TransportationManager()
{
    delete ftpConn;
}

void TransportationManager::connectSignalSlots()
{
    connect(mainWindow->ui.actionConnectServer, SIGNAL(triggered()),
            this, SLOT(OnConnectServer()));
    connect(mainWindow->ui.actionDisconnectServer, SIGNAL(triggered()),
            this, SLOT(OnDisonnectServer()));
    connect(mainWindow->ui.actionUpload, SIGNAL(triggered()),
            this, SLOT(OnUploadProject()));
    connect(mainWindow->ui.actionDonwload, SIGNAL(triggered()),
            this, SLOT(OnDownloadProject()));
}

void TransportationManager::OnConnectServer()
{
    if (Connected == this->connState) {
        QMessageBox::information(mainWindow, tr("提示"), tr("请先断开连接。"));
        return;
    }

    if (NULL == ftpConn)
    {
        ftpConn = new QFtp();
    }
    
    ConnectServerDialog connSvrDlg(&connList, ftpConn);
    connSvrDlg.exec();
    this->connState = connSvrDlg.getState();
}

void TransportationManager::OnDisonnectServer()
{
    QMessageBox::information(mainWindow, "disconnect", "disconnect");
}

void TransportationManager::OnUploadProject()
{
    QMessageBox::information(mainWindow, "upload", "upload");
}

void TransportationManager::OnDownloadProject()
{
    QMessageBox::information(mainWindow, "download", "download");
}

void TransportationManager::initConnectionList()
{
    this->connList.clear();

    ConnectionRecord record1;
    record1.ip = "192.168.1.100";
    record1.port = "14174";
    connList.push_back(record1);

    ConnectionRecord record2;
    record1.ip = "127.0.0.1";
    record1.port = "21";
    connList.push_back(record1);
}


#include <QMessageBox>
#include "Dialog/ConnectServerDialog.h"
#include "Dialog/DownloadProjectDialog.h"
#include "Dialog/UploadProjectDialog.h"
#include "MainWindow.h"
#include "Records/ConnectionRecord.h"
#include "TransportationManager.h"

TransportationManager::TransportationManager(MainWindow* window, QObject* parent)
    : QObject(parent), mainWindow(window), ftp(NULL)
{
    connList.clear();
    connectSignalSlots();
    initServerList();
}

TransportationManager::~TransportationManager()
{
    delete ftp;
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
    if (NULL != ftp)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
    }
    ftp = new QFtp();
    connect(ftp, SIGNAL(commandFinished(int, bool)),
            this, SLOT(ftpCmdFinished(int, bool)));

    ConnectServerDialog connSvrDlg(&connList, ftp, this->mainWindow);
    connSvrDlg.exec();
}

void TransportationManager::OnDisonnectServer()
{
    if (NULL==ftp || (ftp->state() != QFtp::Connected && ftp->state() != QFtp::LoggedIn))
    {
        QMessageBox::information(0, tr("提示"), tr("未登录到服务器。"));
        return;
    }
    else
    {
        ftp->close();
    }
}

void TransportationManager::OnUploadProject()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn)
    {
        QMessageBox::information(0, tr("提示"), tr("请先登录服务器。"));
        return;
    }
    UploadProjectDialog uploadDlg(ftp, this->mainWindow);
    uploadDlg.exec();
}

void TransportationManager::OnDownloadProject()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn)
    {
        QMessageBox::information(0, tr("提示"), tr("请先登录服务器。"));
        return;
    }
    DownloadProjectDialog downloadDlg(ftp, this->mainWindow);
    downloadDlg.exec();
}

void TransportationManager::initServerList()
{
    this->connList.clear();

    //Todo: here should be parse the server config file to fill up the connList.
    ConnectionRecord record1;
    record1.ip = "192.168.1.103";
    record1.port = "21";
    connList.push_back(record1);
    ConnectionRecord record2;
    record1.ip = "127.0.0.1";
    record1.port = "8086";
    connList.push_back(record1);
}

void TransportationManager::ftpCmdFinished(int id, bool error)
{
    //when connect to host finished
    if (ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            this->mainWindow->statusBar()->showMessage(tr("服务器错误"), 500);
            QMessageBox::information(0, tr("错误！"), tr("无法连接服务器，请检查ip和端口"));
            ftp->abort();
            return;
        }
        else
        {
            this->mainWindow->statusBar()->showMessage(tr("找到服务器"), 500);
            return;
        }
    }
    //when login to server finished.
    if (ftp->currentCommand() == QFtp::Login)
    {
        if (error)
        {
            this->mainWindow->statusBar()->showMessage(tr("登录失败"), 2000);
            QMessageBox::information(0, "登录错误", "用户或密码不正确。");
            ftp->abort();
            return;
        } else
        {
            this->mainWindow->statusBar()->showMessage(tr("已登录"), 2000);
            return;
        }
    }
    //when dieconnect server finished.
    if (ftp->currentCommand() == QFtp::Close)
    {
        if (error)
        {
            this->mainWindow->statusBar()->showMessage(tr("断开连接失败"), 2000);
            ftp->abort();
            return;
        } else
        {
            this->mainWindow->statusBar()->showMessage(tr("断开连接"), 2000);
            QMessageBox::information(0, "提示", "断开服务器连接。");
        }
    }
}


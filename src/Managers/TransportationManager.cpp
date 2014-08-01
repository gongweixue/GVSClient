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
        QMessageBox::information(0, tr("��ʾ"), tr("δ��¼����������"));
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
        QMessageBox::information(0, tr("��ʾ"), tr("���ȵ�¼��������"));
        return;
    }
    UploadProjectDialog uploadDlg(ftp, this->mainWindow);
    uploadDlg.exec();
}

void TransportationManager::OnDownloadProject()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn)
    {
        QMessageBox::information(0, tr("��ʾ"), tr("���ȵ�¼��������"));
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
            this->mainWindow->statusBar()->showMessage(tr("����������"), 500);
            QMessageBox::information(0, tr("����"), tr("�޷����ӷ�����������ip�Ͷ˿�"));
            ftp->abort();
            return;
        }
        else
        {
            this->mainWindow->statusBar()->showMessage(tr("�ҵ�������"), 500);
            return;
        }
    }
    //when login to server finished.
    if (ftp->currentCommand() == QFtp::Login)
    {
        if (error)
        {
            this->mainWindow->statusBar()->showMessage(tr("��¼ʧ��"), 2000);
            QMessageBox::information(0, "��¼����", "�û������벻��ȷ��");
            ftp->abort();
            return;
        } else
        {
            this->mainWindow->statusBar()->showMessage(tr("�ѵ�¼"), 2000);
            return;
        }
    }
    //when dieconnect server finished.
    if (ftp->currentCommand() == QFtp::Close)
    {
        if (error)
        {
            this->mainWindow->statusBar()->showMessage(tr("�Ͽ�����ʧ��"), 2000);
            ftp->abort();
            return;
        } else
        {
            this->mainWindow->statusBar()->showMessage(tr("�Ͽ�����"), 2000);
            QMessageBox::information(0, "��ʾ", "�Ͽ����������ӡ�");
        }
    }
}


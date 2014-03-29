#ifndef TRANSPORTATIONMANAGER_H
#define TRANSPORTATIONMANAGER_H

#include <QObject>
#include <vector>
#include <QFtp>
#include "Records/ConnectionRecord.h"
#include "Dialog/ConnectServerDialog.h"
using namespace std;

class MainWindow;

class TransportationManager : public QObject
{
    Q_OBJECT

public:
    TransportationManager(MainWindow* window, QObject *parent = 0);
    ~TransportationManager();

private slots:
    void OnConnectServer();
    void OnDisonnectServer();
    void OnUploadProject();
    void OnDownloadProject();
    void ftpCmdFinished(int id, bool error);

private:
    TransportationManager(QObject *parent);//not impelement.
    void connectSignalSlots();
    void initServerList();
private:
    MainWindow* mainWindow;
    vector<ConnectionRecord> connList;
    QFtp* ftp;
};

#endif // TRANSPORTATIONMANAGER_H

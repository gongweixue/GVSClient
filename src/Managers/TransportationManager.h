#ifndef TRANSPORTATION_MANAGER_H
#define TRANSPORTATION_MANAGER_H

#include <vector>
#include <QFtp>
#include <QObject>
#include "Dialog/ConnectServerDialog.h"
#include "Records/ConnectionRecord.h"

using namespace std;

class MainWindow;

class TransportationManager : public QObject
{
    Q_OBJECT

public:
    TransportationManager(MainWindow* window, QObject* parent = 0);
    ~TransportationManager();

private slots:
    void OnConnectServer();
    void OnDisonnectServer();
    void OnUploadProject();
    void OnDownloadProject();
    void ftpCmdFinished(int id, bool error);

private:
    TransportationManager(QObject* parent);//not impelement.
    void connectSignalSlots();
    void initServerList();
private:
    MainWindow* mainWindow;
    vector<ConnectionRecord> connList;
    QFtp* ftp;
};

#endif // TRANSPORTATION_MANAGER_H

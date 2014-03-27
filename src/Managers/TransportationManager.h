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

private:
    TransportationManager(QObject *parent);//not impelement.
    void connectSignalSlots();
    void initConnectionList();
private:
    MainWindow* mainWindow;
    ConnectState connState;
    vector<ConnectionRecord> connList;
    QFtp* ftpConn;
};

#endif // TRANSPORTATIONMANAGER_H

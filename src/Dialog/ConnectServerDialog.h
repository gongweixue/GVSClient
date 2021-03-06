#ifndef CONNECT_SERVER_DIALOG_H
#define CONNECT_SERVER_DIALOG_H

#include <vector>
#include <QDialog>
#include <QFtp>
#include <QListWidgetItem>
#include "ui_ConnectServerDialog.h"
#include "Records/ConnectionRecord.h"




using namespace std;

class ConnectServerDialog : public QDialog {
    Q_OBJECT

public:
    ConnectServerDialog(std::vector<ConnectionRecord>* connList,
                        QFtp* ftpConn,
                        QWidget* parent = 0);
    ~ConnectServerDialog();

private slots:
    void OnClickOk();
    void OnClickCancle();

private:
    ConnectServerDialog(QWidget* parent = 0);
    void init();
    void connectSignalSlots();

private:
    Ui::ConnectServerDialog ui;
    vector<ConnectionRecord>* pSvrList;
    QFtp* ftp;
    vector<QListWidgetItem> vecOfItem;
};

#endif // CONNECTSERVERDIALOG_H

#include "ConnectServerDialog.h"
#include <QList>
#include <QMessageBox>

using namespace std;

ConnectServerDialog::ConnectServerDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    throw std::exception("The method should not be used normally.");
}

ConnectServerDialog::ConnectServerDialog(vector<ConnectionRecord>* connList,
                                         QFtp* ftpConn,
                                         QWidget *parent)
{
    ui.setupUi(this);
    ftp = ftpConn;
    pSvrList = connList;
    init();
}

ConnectServerDialog::~ConnectServerDialog()
{

}

void ConnectServerDialog::OnClickOk()
{
    //get the selected items of the list of serverlist widget.
    QList<QListWidgetItem*> itemsSelected = ui.serverList->selectedItems();

    if (0 == itemsSelected.size()
        || ui.usrNameEdit->text().isEmpty()
        || ui.passwdEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("连接信息不完整。"));
    }
    else
    {
        QString qstr = itemsSelected[0]->text();
        string str = qstr.toStdString();
        const char* itemStr = str.c_str();
        char itemIdChar[2] = {itemStr[0], '\0'};
        int itemIndex = QString(itemIdChar).toInt();
        ConnectionRecord connRecord = (*pSvrList)[itemIndex];
        //get ip and port
        QString ipStr(connRecord.ip.c_str());
        QString portStr(connRecord.port.c_str());

        //connect and login
        ftp->connectToHost(ipStr, portStr.toUShort());
        ftp->login(ui.usrNameEdit->text(), ui.passwdEdit->text());
        QString usrname = ui.usrNameEdit->text();
        QString psswd = ui.passwdEdit->text();
        this->hide();
    }
}

void ConnectServerDialog::OnClickCancle()
{
    this->close();
}

void ConnectServerDialog::init()
{
    //init the list widget of servers
    vector<ConnectionRecord>::const_iterator iterConRcrd;
    int id = 0;
    for (iterConRcrd = pSvrList->begin(); iterConRcrd != pSvrList->end(); iterConRcrd++)
    {
        QString itemName(QString::number(id) + ". ");
        itemName.append(iterConRcrd->ip.c_str());
        itemName.append(":");
        itemName.append(iterConRcrd->port.c_str());
        QListWidgetItem item(itemName);
        vecOfItem.push_back(item);
        ++id;
    }
    vector<QListWidgetItem>::iterator iterItem;
    for (iterItem = vecOfItem.begin(); iterItem != vecOfItem.end(); iterItem++)
    {
        ui.serverList->addItem(&(*iterItem));
    }
    //init the signale and slots
    connectSignalSlots();
}

void ConnectServerDialog::connectSignalSlots()
{
    //ok btn;
    connect(ui.okBtn, SIGNAL(clicked()), this, SLOT(OnClickOk()));
    //cancle btn;
    connect(ui.cancleBtn, SIGNAL(clicked()), this, SLOT(OnClickCancle()));
}

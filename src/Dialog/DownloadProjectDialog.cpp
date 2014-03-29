#include "DownloadProjectDialog.h"
#include <QMessageBox>

DownloadProjectDialog::DownloadProjectDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    throw std::exception("The method or operation is not implemented.");
}

DownloadProjectDialog::DownloadProjectDialog( QFtp* pFtp, QWidget *parent)
{
    ui.setupUi(this);
    ftp = pFtp;
    init();
}

DownloadProjectDialog::~DownloadProjectDialog()
{
}

void DownloadProjectDialog::init()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn) {
        QMessageBox::information(0, tr("��ʾ"), tr("��δ���ڵ�¼״̬,���ȵ�¼��"));
        ui.downLoadBtn->setEnabled(false);
        return;
    }
    ui.projectList->setHeaderLabels(QStringList()<<tr("����")<<tr("��С")<<tr("ʱ��"));
    initSignalSlots();
    ftp->cd("/");
    ftp->list();
}

void DownloadProjectDialog::initSignalSlots()
{
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinished(int, bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(addToList(const QUrlInfo&)));
}

void DownloadProjectDialog::ftpCmdFinished(int id, bool error)
{
    if (error) {
        QMessageBox::information(0, "", ftp->errorString());
    }
}

void DownloadProjectDialog::addToList( const QUrlInfo& urlInfo )
{
    //make a item of list
    //add to list widget
}


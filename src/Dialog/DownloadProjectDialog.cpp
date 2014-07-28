#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "DownloadProjectDialog.h"

using namespace std;

DownloadProjectDialog::DownloadProjectDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    throw std::exception("The method or operation is not implemented.");
}

DownloadProjectDialog::DownloadProjectDialog(QFtp* pFtp, QWidget *parent)
{
    ui.setupUi(this);
    ftp = pFtp;
    init();
    connect(ui.downLoadBtn, SIGNAL(clicked(bool)), this, SLOT(OnClickDownload(bool)));
}

DownloadProjectDialog::~DownloadProjectDialog()
{
}

void DownloadProjectDialog::init()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn)
    {
        QMessageBox::information(0, tr("提示"), tr("您未处于登录状态,请先登录。"));
        ui.downLoadBtn->setEnabled(false);
        return;
    }

    ui.projectList->setHeaderLabels(QStringList() << tr("名称") << tr("时间"));

    initPrjList();
}

void DownloadProjectDialog::initPrjList()
{
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(fillPrjList(const QUrlInfo&)));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinished(int, bool)));
    ftp->cd("/");
    initListCmdID = ftp->list();
}

void DownloadProjectDialog::fillPrjList( const QUrlInfo& urlInfo )
{
    if (urlInfo.isFile())
    {
        qint64 size = urlInfo.size();
        QDateTime modifiedTime = urlInfo.lastModified();
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, urlInfo.name());
        item->setText(1, urlInfo.lastModified().toString("hh:mm:ss yyyy-MM-dd"));
        ui.projectList->addTopLevelItem(item);
    }
}

void DownloadProjectDialog::installSignalSlots()
{
    //disconnect fillPrjList with listInfo signal if it exists.
    disconnect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(fillPrjList(const QUrlInfo&)));

    connect(ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(ftpListInfo(const QUrlInfo&)));
}

void DownloadProjectDialog::uninstallSignalSlots()
{
    disconnect(ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    disconnect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(OnListInfo(const QUrlInfo&)));
}

void DownloadProjectDialog::OnClickDownload(bool checked)
{
    installSignalSlots();

    QList<QTreeWidgetItem*> items = (ui.projectList->selectedItems());
    if (items.isEmpty())
    {
        goto bail;
    }
    else
    {
        //get the name of project
        QString fileName = items[0]->text(0);
        QStringList strSplit = fileName.split('.');
        QString projectName;
        for (int i = 0; i < strSplit.count() - 1; ++i)
        {
            projectName.append(strSplit[i]);
        }

        //get the path to store
        QString localToStore = QFileDialog::getExistingDirectory(this,
            tr("选择存放位置"),
            tr("d:/"),
            QFileDialog::ShowDirsOnly
            | QFileDialog::HideNameFilterDetails);
        if (localToStore.isEmpty())
        {
            goto bail;
        }
        localToStore.append("/");

        ftp->cd("/");

        downloadProjectToLocal(fileName, localToStore);
    }

bail:
    uninstallSignalSlots();
    return;
}

void DownloadProjectDialog::downloadProjectToLocal(QString fileName, QString localToStore)
{
    QStringList strSplit = fileName.split('.');
    QString projectName;
    for (int i = 0; i < strSplit.count() - 1; ++i)
    {
        projectName.append(strSplit[i]);
    }

    //down project file
    projectFile.setFileName(localToStore + fileName);
    
    if (!projectFile.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, tr("错误"), tr("无法创建项目") + projectName);
        ftp->abort();
        return;
    }
    ftp->cd("/");
    ftp->get(fileName, &projectFile);

     //create prj dir.
     QString strPrjDataDir(localToStore + projectName + "/");
     localDir = localToStore;
     //QDir().mkdir(strPrjDataDir);
     //cd prj data dir remote.
     //ftp->cd(projectName);

     //get remote prj data
     QString remoteDir("/" + projectName + "/");
     getDirectory(remoteDir);
}

void DownloadProjectDialog::getDirectory(QString remotePrjDataDir)
{
    pendingDirs.append(remotePrjDataDir);
    processNextDirectory();
}

void DownloadProjectDialog::ftpDone(bool error)
{
    if (error)
    {
        QMessageBox::information(this, tr("出错啦"), ftp->errorString());
        ftp->cd("/");
        ftp->abort();
        return;
    }

    qDeleteAll(openedFiles);
    openedFiles.clear();

    processNextDirectory();
}

void DownloadProjectDialog::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile())
    {
        if (urlInfo.isReadable())
        {
            QString fileName(urlInfo.name());
            QFile *file = new QFile(currentLocalDir + "/" + fileName);
            if (!file->open(QIODevice::WriteOnly))
            {
                QMessageBox::information(this, tr("错误"), tr("无法拷贝") + fileName);
                ftp->abort();
                return;
            }

            ftp->get(fileName, file);
            openedFiles.append(file);
        }
    }
    else if (urlInfo.isDir() && !urlInfo.isSymLink())
    {
        pendingDirs.append(currentDir + "/" + urlInfo.name());
    }
}

void DownloadProjectDialog::processNextDirectory()
{
    if (!pendingDirs.isEmpty())
    {
        currentDir = pendingDirs.takeFirst();
        currentLocalDir = localDir + currentDir;
        QDir().mkpath(currentLocalDir);

        ftp->cd(currentDir);
        ftp->list();
    }
    else
    {
        emit done();
    }
}




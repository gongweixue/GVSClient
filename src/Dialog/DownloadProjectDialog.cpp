#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include "DownloadProjectDialog.h"
#include "quazip.h"
#include "quazipfile.h"

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

    initSignalSlot();
    initPrjList();
}

void DownloadProjectDialog::initSignalSlot()
{
    connect(ui.downLoadBtn, SIGNAL(clicked(bool)), this, SLOT(OnClickDownload(bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)),
            this, SLOT(fillPrjList(const QUrlInfo&)));
    connect(ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
}

void DownloadProjectDialog::initPrjList()
{
    ftp->cd("/");
    ftp->list();
}

void DownloadProjectDialog::fillPrjList( const QUrlInfo& urlInfo )
{
    if (urlInfo.isFile())
    {
        qint64 size = urlInfo.size();
        QDateTime modifiedTime = urlInfo.lastModified();
        QTreeWidgetItem* item = new QTreeWidgetItem();
        QStringList list = urlInfo.name().split(".");
        QString prjName;
        for (int i = 0; i < list.count() - 1; ++i)
        {
            prjName.append(list.at(i));
        }
        item->setText(0, prjName);
        item->setText(1, urlInfo.lastModified().toString("hh:mm:ss yyyy-MM-dd"));
        ui.projectList->addTopLevelItem(item);
    }
}

void DownloadProjectDialog::OnClickDownload(bool checked)
{
    QList<QTreeWidgetItem*> items = (ui.projectList->selectedItems());
    if (items.isEmpty())
    {
        goto bail;
    }
    else
    {
        //get the name of project
        QString projectName(items[0]->text(0));

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
        this->ui.downLoadBtn->setEnabled(false);
        localToStore.append("/");
        compressFileName = projectName + ".zip";
        this->localToStore = localToStore;

        ftp->cd("/");
        downloadCompressPrjToLocal(projectName, localToStore);
    }
bail:
    return;
}

void DownloadProjectDialog::downloadCompressPrjToLocal(QString projectName, QString localToStore)
{
    //down project file
    compressFile.setFileName(localToStore + projectName + ".zip");
    
    if (!compressFile.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, tr("错误"), tr("无法创建项目文件") + projectName);
        ftp->abort();
        return;
    }
    ftp->cd("/");
    ftp->get(projectName + ".zip", &compressFile);
}

void DownloadProjectDialog::ftpDone(bool error)
{
    if (error)
    {
        QMessageBox::information(this, tr("出错啦"), ftp->errorString());
        ftp->cd("/");
        ftp->abort();
    }
    this->compressFile.close();


    if (localToStore.isEmpty() == false && compressFileName.isEmpty() == false)
    {
        uncompressPrj();
        if (!QFile::remove(localToStore + compressFileName))
        {
            QMessageBox::information(NULL, tr("错误"), tr("移除archive失败。"));
        }
    }

    compressFileName = "";
    localToStore = "";
    this->ui.downLoadBtn->setEnabled(true);
}

bool DownloadProjectDialog::uncompressPrj()
{
    QString compressFileFullPath(localToStore + compressFileName);

    QuaZip archive(compressFileFullPath);
    if (!archive.open(QuaZip::mdUnzip))
    {
        QMessageBox::information(NULL, tr("错误"), tr("打开：") + compressFileFullPath);
    }

    QString path = localToStore;
    if (!path.endsWith("/") && !localToStore.endsWith("\\"))
        path += "/";

    QDir dir(localToStore);
    if (!dir.exists())
        dir.mkpath(localToStore);

    for(bool f = archive.goToFirstFile(); f; f = archive.goToNextFile())
    {
        QString filePath = archive.getCurrentFileName();
        QuaZipFile zFile(archive.getZipName(), filePath);
        zFile.open(QIODevice::ReadOnly );
        QByteArray ba = zFile.readAll();
        zFile.close();

        if (filePath.endsWith("/"))
        {
            dir.mkpath(filePath);
        }
        else
        {
            QFile dstFile(path + filePath);
            if (!dstFile.open(QIODevice::WriteOnly))
            {
                QMessageBox::information(NULL, tr("错误"), tr("写入") + path + filePath);
                return false;
            }
            dstFile.write(ba);
            dstFile.close();
        }
    }

    return true;
}





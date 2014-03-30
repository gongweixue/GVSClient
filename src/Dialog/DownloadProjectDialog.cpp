#include "DownloadProjectDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

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
    this->projectFile = NULL;
    modelFileList.clear();
    idListModels = -1;
    isInModelDir = false;
    init();
}

DownloadProjectDialog::~DownloadProjectDialog()
{
    clearQFiles();
}

void DownloadProjectDialog::init()
{
    if (NULL == ftp || ftp->state() != QFtp::LoggedIn) {
        QMessageBox::information(0, tr("提示"), tr("您未处于登录状态,请先登录。"));
        ui.downLoadBtn->setEnabled(false);
        return;
    }

    ui.projectList->setHeaderLabels(QStringList() << tr("名称") << tr("时间"));
    initSignalSlots();
    ftp->cd("/");
    ftp->list();
}

void DownloadProjectDialog::initSignalSlots()
{
    connect(ftp, SIGNAL(done(bool)), this, SLOT(OnDone(bool)));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinished(int, bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(OnListInfo(const QUrlInfo&)));
    connect(ui.downLoadBtn, SIGNAL(clicked(bool)), this, SLOT(OnClickDownload(bool)));
}

void DownloadProjectDialog::ftpCmdFinished(int id, bool error)
{
    if (error) {
        QMessageBox::information(0, "", ftp->errorString());
        ftp->cd("/");
        ftp->abort();
        return;
    }
    if (id == idListModels)
    {
        //down model files, push into list
        QStringList strSplit = strProjectFileName.split('.');
        QString projectName;
        for (int i = 0; i < strSplit.count() - 1; ++i)
        {
            projectName.append(strSplit[i]);
        }
        QString pathModel = strLocalPath + projectName + "/";
        QDir().mkdir(pathModel);
        for (int i = 0; i < modelNameList.count(); ++i)
        {

            QFile* pFile = new QFile(pathModel + modelNameList[i]);
            modelFileList.push_back(pFile);
            if (!pFile->open(QIODevice::ReadWrite)) {
                QMessageBox::information(this, tr("错误"), ftp->errorString());
                ftp->abort();
                ftp->cd("/");
                return;
            }
            QString modelName = modelNameList[i];
            ftp->get(modelName, pFile);
        }
        ftp->cd("/");
    }
}

void DownloadProjectDialog::addProjectItem( const QUrlInfo& urlInfo )
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

void DownloadProjectDialog::OnClickDownload(bool checked)
{
    QList<QTreeWidgetItem*> items = (ui.projectList->selectedItems());
    if (items.isEmpty())
        return;

    //get the name of project
    QString fileName = items[0]->text(0);
    QStringList strSplit = fileName.split('.');
    QString projectName;
    for (int i = 0; i < strSplit.count() - 1; ++i)
    {
        projectName.append(strSplit[i]);
    }

    //get the path to store
    QString pathToStore = QFileDialog::getExistingDirectory(this,
                                  tr("选择存放位置"),
                                  tr("d:/"),
                                  QFileDialog::ShowDirsOnly
                                  | QFileDialog::HideNameFilterDetails);
    if (pathToStore.isEmpty())
    {
        return;
    }
    pathToStore.append("/");

    ftp->cd("/");
    isInModelDir = false;

    downLoadFiles(fileName, pathToStore);
}

void DownloadProjectDialog::clearQFiles()
{
    if (projectFile)
    {
        projectFile->close();
        delete projectFile;
        projectFile = NULL;
    }
    for (int i = 0; i < modelFileList.count(); ++i) {
        if (modelFileList[i]) {
            modelFileList[i]->close();
            delete modelFileList[i];
            modelFileList[i] = NULL;
        }
    }
}

void DownloadProjectDialog::OnDone(bool err)
{
    if (err) {
        QMessageBox::information(this, tr("出错啦"), ftp->errorString());
        ftp->cd("/");
        ftp->abort();
        return;
    }

    clearQFiles();
    ui.downLoadBtn->setEnabled(true);
}

void DownloadProjectDialog::downLoadFiles( QString fileName, QString pathToStore )
{
    strLocalPath = pathToStore;
    strProjectFileName = fileName;

    QStringList strSplit = fileName.split('.');
    QString projectName;
    for (int i = 0; i < strSplit.count() - 1; ++i)
    {
        projectName.append(strSplit[i]);
    }
    //down project file
    projectFile = new QFile(pathToStore + fileName);
    if (!projectFile->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("错误"), tr("无法创建项目") + projectName);
        ftp->abort();
        return;
    }
    ftp->cd("/");
    ftp->get(fileName, projectFile);

    //cd model dir
    ftp->cd(projectName);
    isInModelDir = true;
    idListModels = ftp->list();

    ui.downLoadBtn->setEnabled(false);
}

void DownloadProjectDialog::OnListInfo(const QUrlInfo& urlInfo)
{
    if (!isInModelDir)
    {
        addProjectItem(urlInfo);
    }
    else
    {
        modelNameList.push_back(urlInfo.name());
    }
}

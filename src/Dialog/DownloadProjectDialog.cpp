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
    //ui.downLoadBtn->setEnabled(false);/*this should be open when no item selected.*/
    init();
}

DownloadProjectDialog::~DownloadProjectDialog()
{
    clearQFiles();
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
    connect(ftp, SIGNAL(done(bool)), this, SLOT(OnDone(bool)));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(ftpCmdFinished(int, bool)));
    connect(ftp, SIGNAL(listInfo(const QUrlInfo&)), this, SLOT(addProjectItem(const QUrlInfo&)));
    connect(ui.downLoadBtn, SIGNAL(clicked(bool)), this, SLOT(OnDownloadProject(bool)));
}

void DownloadProjectDialog::ftpCmdFinished(int id, bool error)
{
    if (error) {
        QMessageBox::information(0, "", ftp->errorString());
    }
}

void DownloadProjectDialog::addProjectItem( const QUrlInfo& urlInfo )
{
    //make a item of list
    //add to list widget
}

void DownloadProjectDialog::OnDownloadProject(bool checked)
{
    //get the item chose, if no item chosed, then return.
    //if (list has item chosed)
    //    get the name of project
    //else
    //return;
    //get the name of project
    QString fileName = "aaa.gvp";
    QString projectName = "aaa";

    //get model file list
    QStringList modelNameList;//#######################list(), to trigger list info
    //!!!mock some model name
    modelNameList.push_back("1.txt");
    modelNameList.push_back("2.txt");
    modelNameList.push_back("3.txt");



    //get the path to store
    QString pathToStore = QFileDialog::getExistingDirectory(this,
                                  tr("ѡ����λ��"),
                                  tr("d:/"),
                                  QFileDialog::ShowDirsOnly
                                  | QFileDialog::HideNameFilterDetails);
    pathToStore.append("/");

    //down project file
    projectFile = new QFile(pathToStore + fileName);
    if (!projectFile->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("����"), tr("�޷�������Ŀ") + projectName);
        ftp->abort();
        return;
    }
    ftp->cd("/");
    ftp->get(fileName, projectFile);

    //cd model dir
    ftp->cd(projectName);


    //down model files, push into list
    QString pathModel = pathToStore + projectName + "/";
    QDir().mkdir(pathModel);
    for (int i = 0; i < modelNameList.count(); ++i)
    {
        
        QFile* pFile = new QFile(pathModel + modelNameList[i]);
        modelFileList.push_back(pFile);
        if (!pFile->open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("����"), ftp->errorString());
            ftp->abort();
            ftp->cd("/");
            return;
        }
        QString modelName = modelNameList[i];
        ftp->get(modelName, pFile);
    }

    //cd to root
    ftp->cd("/");
    ui.downLoadBtn->setEnabled(false);
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
        QMessageBox::information(this, tr("������"), ftp->errorString());
    }
    clearQFiles();
    ui.downLoadBtn->setEnabled(true);
}


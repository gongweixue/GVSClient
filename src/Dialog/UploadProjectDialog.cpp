#include "UploadProjectDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <vector>

UploadProjectDialog::UploadProjectDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    throw std::exception("The method or operation is not implemented.");
}

UploadProjectDialog::UploadProjectDialog(QFtp* pFtp, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->ftp = pFtp;
    this->projectFile = NULL;
    modelFileList.clear();
    ui.progressBar->setVisible(false);
    ui.upLoadBtn->setEnabled(false);
    init();
}

UploadProjectDialog::~UploadProjectDialog()
{
    //delete list files and project file
    clearQFiles();
}

void UploadProjectDialog::init()
{
    connect(ui.browseBtn, SIGNAL(clicked()), this, SLOT(OnBrowse()));
    connect(ui.upLoadBtn, SIGNAL(clicked()), this, SLOT(OnUpload()));
    connect(ftp, SIGNAL(done(bool)), this, SLOT(OnDone(bool)));
    connect(ftp, SIGNAL(commandFinished(int, bool)),
            this, SLOT(OnCmdFinished(int, bool)));
    connect(ui.pathEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(EnableUploadBtn(const QString&)));
}

void UploadProjectDialog::OnBrowse()
{
    QString projectName = QFileDialog::getOpenFileName(this,
                                  tr("选择上传的项目"),
                                  tr("D:/"),
                                  tr("GVP项目文件(*.gvp)"),
                                  &tr("GVP项目文件(*.gvp)"));
    this->ui.pathEdit->setText(projectName);
}

void UploadProjectDialog::OnUpload()
{
    //project file
    QString projectFullName = ui.pathEdit->text();
    if (ui.pathEdit->text().isEmpty())
    {
        return;
    }
    projectFile = new QFile(projectFullName);
    if (!projectFile->open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("错误"), tr("无法打开项目") + projectFullName);
        ftp->abort();
        return;
    }
    QFileInfo projectFileInfo(projectFullName);
    QString strProjectName = projectFileInfo.fileName();
    ftp->put(projectFile, strProjectName);

    //model files
    QString strBaseName = projectFileInfo.baseName();
    QString strProjectDir = projectFileInfo.absolutePath();
    strProjectDir.append("/");
    QString strModelFilesDir = strProjectDir + strBaseName + "/";

    QDir modelDir(strModelFilesDir);
    QStringList modelNameList = modelDir.entryList(QDir::Files);
    ftp->mkdir(strBaseName);
    ftp->cd(tr("/").append(strBaseName));
    modelFileList.clear();
    for (int i = 0; i < modelNameList.count(); ++i)
    {
        QFile* pFile = new QFile(strModelFilesDir + modelNameList[i]);
        modelFileList.push_back(pFile);
        if (!pFile->open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("错误"), tr("无法打开文件") + modelNameList[i]);
            ftp->abort();
            ftp->cd("/");
            return;
        }
        QString modelName = modelNameList[i];
        ftp->put(pFile, modelName);
    }
    ftp->cd("/");
    ui.browseBtn->setDisabled(true);
    ui.upLoadBtn->setDisabled(true);
    ui.progressBar->setMinimum(0);
    ui.progressBar->setMaximum(modelNameList.count());
}

void UploadProjectDialog::OnDone(bool err)
{
    if (err)
    {
        QMessageBox::information(this,
                                 tr("出错啦"),
                                 tr("文件上传失败。").append(ftp->errorString()));
    }
    ui.browseBtn->setEnabled(true);
    ui.upLoadBtn->setEnabled(true);
    ui.progressBar->setValue(0);
    clearQFiles();
}

void UploadProjectDialog::OnCmdFinished(int id, bool err)
{
    if (QFtp::Put == ftp->currentCommand())
    {
        ui.progressBar->setValue(ui.progressBar->value());
    }
}

void UploadProjectDialog::clearQFiles()
{
    if (projectFile)
    {
        projectFile->close();
        delete projectFile;
        projectFile = NULL;
    }
    for (int i = 0; i < modelFileList.count(); ++i)
    {
        if (modelFileList[i])
        {
            modelFileList[i]->close();
            delete modelFileList[i];
            modelFileList[i] = NULL;
        }
    }
}

void UploadProjectDialog::EnableUploadBtn(const QString& text)
{
    if (text.isEmpty())
    {
        ui.upLoadBtn->setEnabled(false);
    }
    else
    {
        ui.upLoadBtn->setEnabled(true);
    }
}


#include <vector>
#include <QFileDialog>
#include <QMessageBox>
#include "3rdparty/quazip/quazipfile.h"
#include "3rdparty/quazip/quazip.h"
#include "UploadProjectDialog.h"

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
    this->compressFile = NULL;
    ui.upLoadBtn->setEnabled(false);
    init();
}

UploadProjectDialog::~UploadProjectDialog()
{
    delete this->compressFile;
    this->compressFile = NULL;
}

void UploadProjectDialog::init()
{
    connect(ui.browseBtn, SIGNAL(clicked()), this, SLOT(OnBrowse()));
    connect(ui.upLoadBtn, SIGNAL(clicked()), this, SLOT(OnUpload()));
    connect(ftp, SIGNAL(done(bool)), this, SLOT(OnDone(bool)));
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
    //check if file exists, and the suffix is gvp.
    QString gvpPath = this->ui.pathEdit->text();
    QFileInfo gvpFI(gvpPath);
    if (!gvpFI.exists())
    {
        QMessageBox::information(NULL, tr("错误"), tr("文件不存在:") + gvpPath);
        return;
    }
    if (0 != gvpFI.suffix().compare(tr("gvp")))
    {
        QMessageBox::information(NULL, tr("错误"), tr("非法工程：") + gvpPath);
        return;
    }
    QStringList split = ui.pathEdit->text().split(".");
    QString prjDirPath;
    for (int i = 0; i < split.count() - 2; ++i)
    {
        prjDirPath.append(split[i] + ".");
    }
    prjDirPath.append(split[split.count() - 2]);
    QFileInfo prjDirFI(prjDirPath);
    if (!gvpFI.exists())
    {
        QMessageBox::information(NULL, tr("错误"), tr("文件夹不存在：") + prjDirPath);
        return;
    }
    if (!prjDirFI.isDir())
    {
        QMessageBox::information(NULL, tr("错误"), tr("不是文件夹：") + prjDirPath);
        return;
    }

    if (!compressPrj(gvpFI))
    {
        QMessageBox::information(NULL, tr("错误"), tr("压缩工程失败"));
        return;
    }

    QFileInfo compressFI(prjDirFI.absoluteFilePath() + ".zip");
    if (!compressFI.exists())
    {
        QMessageBox::information(NULL, tr("错误"), tr(" 无法获取压缩后的工程"));
        return;
    }
    ui.browseBtn->setEnabled(false);
    ui.upLoadBtn->setEnabled(false);

    compressFile = new QFile(prjDirFI.absoluteFilePath() + ".zip");
    if (!compressFile->open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("错误"), tr("无法打开压缩工程文件"));
        ftp->abort();
        return;
    }
    ftp->cd("/");
    ftp->put(this->compressFile, prjDirFI.fileName() + ".zip");
}

void UploadProjectDialog::OnDone(bool err)
{
    if (err)
    {
        QMessageBox::information(this,
            tr("出错啦"),
            tr("文件上传失败。").append(ftp->errorString()));
    }
    this->compressFile->close();
    if (!QFile::remove(this->compressFile->fileName()))
    {
        QMessageBox::information(NULL, tr("错误"), tr("移除archive失败。"));
    }

    delete this->compressFile;
    this->compressFile = NULL;
    ui.browseBtn->setEnabled(true);
    ui.upLoadBtn->setEnabled(true);

    this->ui.pathEdit->clear();
}

void UploadProjectDialog::EnableUploadBtn(const QString& text)
{
    this->ui.upLoadBtn->setEnabled(!text.isEmpty());
}

void UploadProjectDialog::recurseAddDir(QDir d, QStringList & list)
{

    QStringList qsl = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString file, qsl) {

        QFileInfo finfo(QString("%1/%2").arg(d.path()).arg(file));

        if (finfo.isSymLink())
            return;

        if (finfo.isDir())
        {
            list << QDir::toNativeSeparators(finfo.filePath() + "/");
            QString dirname = finfo.fileName();
            QDir sd(finfo.filePath());

            recurseAddDir(sd, list);

        } 
        else
        {
            list << QDir::toNativeSeparators(finfo.filePath());
        }
    }
}

bool UploadProjectDialog::compressPrj(const QFileInfo& gvpFI)
{
    QString zipFilePath(gvpFI.absolutePath() + "/" + gvpFI.completeBaseName() + ".zip");
    QuaZip zip(zipFilePath);
    zip.setFileNameCodec("IBM866");

    if (!zip.open(QuaZip::mdCreate))
    {
        return false;
    }
    QString dirPath(gvpFI.absolutePath() + "/" + gvpFI.completeBaseName());
    QDir dirBeZipped(dirPath);
    if (!dirBeZipped.exists())
    {
        return false;
    }

    QFile inFile;
    QStringList sl;
    sl.append(gvpFI.absolutePath() + "/" + gvpFI.completeBaseName() + ".gvp");
    recurseAddDir(dirBeZipped, sl);

    QFileInfoList files;
    foreach (QString fn, sl) files << QFileInfo(fn);

    QuaZipFile outFile(&zip);

    char c;
    foreach(QFileInfo fileInfo, files)
    {
        if (!fileInfo.isFile())
            continue;
        int lenToRemoved = gvpFI.absolutePath().length() + 1;
        QString fileNameWithRelativePath = fileInfo.filePath().remove(0, lenToRemoved);

        inFile.setFileName(fileInfo.filePath());

        if (!inFile.open(QIODevice::ReadOnly))
        {
            return false;
        }

        if (!outFile.open(QIODevice::WriteOnly,
            QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath())))
        {
            return false;
        }

        while (inFile.getChar(&c) && outFile.putChar(c));

        if (outFile.getZipError() != UNZ_OK)
        {
            return false;
        }

        outFile.close();

        if (outFile.getZipError() != UNZ_OK)
        {
            return false;
        }

        inFile.close();
    }

    zip.close();

    if (zip.getZipError() != 0)
    {
        return false;
    }

    return true;
}



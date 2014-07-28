#ifndef DOWNLOAD_PROJECT_DIALOG_H
#define DOWNLOAD_PROJECT_DIALOG_H

#include <QDialog>
#include <QFile>
#include <QFtp>
#include <QList>
#include <QStringList>
#include "ui_DownloadProjectDialog.h"

class DownloadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    DownloadProjectDialog(QFtp* pFtp, QWidget* parent = 0);
    ~DownloadProjectDialog();

private:
    void init();
    DownloadProjectDialog(QWidget* parent = 0);
    void installSignalSlots();
    void uninstallSignalSlots();
    void initPrjList();
    void downloadProjectToLocal(QString fileName, QString localToStore);
    void downloadDataToPrjDir(QString PrjDataPath);

private slots:
    void fillPrjList(const QUrlInfo& urlInfo);
    void OnClickDownload(bool checked);
    void ftpListInfo(const QUrlInfo& urlInfo);
    void ftpDone(bool error);
    
private:
    Ui::DownloadProjectDialog ui;
    QFtp* ftp;
    QFile projectFile;
    int initListCmdID;

    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////
private:
    void getDirectory(QString remotePrjDataDir);

signals:
    void done();
private:
    void processNextDirectory();
    QList<QFile *> openedFiles;
    QString currentDir;
    QString currentLocalDir;
    QStringList pendingDirs;
    QString localDir;
};

#endif // DOWNLOAD_PROJECT_DIALOG_H

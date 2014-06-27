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
    void clearQFiles();
    DownloadProjectDialog(QWidget* parent = 0);
    void downLoadFiles(QString, QString);

private slots:
    void initSignalSlots();
    void ftpCmdFinished(int id, bool error);
    void addProjectItem(const QUrlInfo& urlInfo);
    void OnClickDownload(bool checked);
    void OnDone(bool err);
    void OnListInfo(const QUrlInfo& urlInfo);

private:
    Ui::DownloadProjectDialog ui;
    QFtp* ftp;
    QList<QFile*> modelFileList;
    QFile* projectFile;
    QStringList modelNameList;
    int idListModels;
    bool isInModelDir;
    QString strLocalPath;
    QString strProjectFileName;
};

#endif // DOWNLOAD_PROJECT_DIALOG_H

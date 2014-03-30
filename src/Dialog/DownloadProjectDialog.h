#ifndef DOWNLOADPROJECTDIALOG_H
#define DOWNLOADPROJECTDIALOG_H

#include <QDialog>
#include "ui_DownloadProjectDialog.h"
#include <QFtp>
#include <QList>
#include <QFile>
#include <QStringList>

class DownloadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    DownloadProjectDialog(QFtp* pFtp, QWidget *parent = 0);
    ~DownloadProjectDialog();

private:
    void init();
    void clearQFiles();
    DownloadProjectDialog(QWidget *parent = 0);
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

#endif // DOWNLOADPROJECTDIALOG_H

#ifndef DOWNLOADPROJECTDIALOG_H
#define DOWNLOADPROJECTDIALOG_H

#include <QDialog>
#include "ui_DownloadProjectDialog.h"
#include <QFtp>
#include <QList>
#include <QFile>

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

private slots:
    void initSignalSlots();
    void ftpCmdFinished(int id, bool error);
    void addProjectItem(const QUrlInfo& urlInfo);
    void OnDownloadProject(bool checked);
    void OnDone(bool err);
private:
    Ui::DownloadProjectDialog ui;
    QFtp* ftp;
    QList<QFile*> modelFileList;
    QFile* projectFile;
};

#endif // DOWNLOADPROJECTDIALOG_H

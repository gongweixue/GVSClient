#ifndef UPLOADPROJECTDIALOG_H
#define UPLOADPROJECTDIALOG_H

#include <QDialog>
#include "ui_UploadProjectDialog.h"
#include <QFtp>
#include <QFile>

class UploadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    UploadProjectDialog(QWidget *parent = 0);
    UploadProjectDialog(QFtp* pFtp, QWidget *parent = 0);
    ~UploadProjectDialog();

private:
    void init();
    void clearQFiles();

public slots:
    void OnBrowse();
    void OnUpload();
    void OnDone(bool err);
    void OnCmdFinished(int id, bool err);
    void EnableUploadBtn(const QString& text);
private:
    Ui::UploadProjectDialog ui;
    QFtp* ftp;
    QList<QFile*> modelFileList;
    QFile* projectFile;
};

#endif // UPLOADPROJECTDIALOG_H

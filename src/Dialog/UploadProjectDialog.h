#ifndef UPLOAD_PROJECT_DIALOG_H
#define UPLOAD_PROJECT_DIALOG_H

#include <QDialog>
#include "ui_UploadProjectDialog.h"
#include <QFtp>
#include <QFile>

class UploadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    UploadProjectDialog(QFtp* pFtp, QWidget* parent = 0);
    ~UploadProjectDialog();

private:
    UploadProjectDialog(QWidget* parent = 0);
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

#endif // UPLOAD_PROJECT_DIALOG_H

#ifndef UPLOAD_PROJECT_DIALOG_H
#define UPLOAD_PROJECT_DIALOG_H

#include <QDialog>
#include <QFile>
#include <QFtp>
#include "ui_UploadProjectDialog.h"

class QDir;
class QFileInfo;

class UploadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    UploadProjectDialog(QFtp* pFtp, QWidget* parent = 0);
    ~UploadProjectDialog();

private:
    UploadProjectDialog(QWidget* parent = 0);
    void init();
    bool compressPRJFiles(QString prjName);
    static void recurseAddDir(QDir d, QStringList & list);
    static bool compressPrj(const QFileInfo& gvpFI);

public slots:
    void OnBrowse();
    void OnUpload();
    void OnDone(bool err);
    void EnableUploadBtn(const QString& text);

private:
    Ui::UploadProjectDialog ui;
    QFtp* ftp;
    QFile* compressFile;
};

#endif // UPLOAD_PROJECT_DIALOG_H

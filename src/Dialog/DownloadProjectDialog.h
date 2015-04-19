#ifndef DOWNLOAD_PROJECT_DIALOG_H
#define DOWNLOAD_PROJECT_DIALOG_H

#include <QDialog>
#include <QFile>
#include <QFtp>
#include <QList>
#include <QStringList>
#include "ui_DownloadProjectDialog.h"

class DownloadProjectDialog : public QDialog {
    Q_OBJECT

public:
    DownloadProjectDialog(QFtp* pFtp, QWidget* parent = 0);
    ~DownloadProjectDialog();

private:
    DownloadProjectDialog(QWidget* parent = 0);

    void init();
    void initSignalSlot();
    void initPrjList();
    void downloadCompressPrjToLocal(const QString& projectName,
                                    const QString& localToStore);
    bool uncompressPrj();

private slots:
    void fillPrjList(const QUrlInfo& urlInfo);
    void OnClickDownload(bool checked);
    void ftpDone(bool error);
    void getFinished( int id, bool err ) {
        throw std::exception("The method or operation is not implemented.");
    }

private:
    Ui::DownloadProjectDialog ui;
    QFtp* ftp;
    QFile compressFile;
    QString compressFileName;
    QString localToStore;
    int getCMDId;
};

#endif // DOWNLOAD_PROJECT_DIALOG_H

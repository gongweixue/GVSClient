#ifndef DOWNLOADPROJECTDIALOG_H
#define DOWNLOADPROJECTDIALOG_H

#include <QDialog>
#include "ui_DownloadProjectDialog.h"
#include <QFtp>

class DownloadProjectDialog : public QDialog
{
    Q_OBJECT

public:
    DownloadProjectDialog(QWidget *parent = 0);
    DownloadProjectDialog(QFtp* pFtp, QWidget *parent = 0);
    ~DownloadProjectDialog();

private:
    void init();

private slots:
    void initSignalSlots();
    void ftpCmdFinished(int id, bool error);
    void addToList(const QUrlInfo& urlInfo);
private:
    Ui::DownloadProjectDialog ui;
    QFtp* ftp;
};

#endif // DOWNLOADPROJECTDIALOG_H

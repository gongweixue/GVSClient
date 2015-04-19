#ifndef ADDFAVGROUPDIALOG_H
#define ADDFAVGROUPDIALOG_H

#include <QDialog>
#include "ui_AddFavGroupDialog.h"

class AddFavGroupDialog : public QDialog {
    Q_OBJECT

public:
    AddFavGroupDialog(QWidget* parent = 0);
    ~AddFavGroupDialog();

    QString getGroupName() {return ui.GroupNameLineEdit->text();}
    QLineEdit* getGroupNameLineEdit() {return ui.GroupNameLineEdit;}
    QPushButton* getOkBtn() {return ui.okButton;}

private:
    Ui::AddFavGroupDialog ui;
};

#endif // ADDFAVGROUPDIALOG_H

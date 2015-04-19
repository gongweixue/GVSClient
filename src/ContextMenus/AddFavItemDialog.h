#ifndef ADDFAVITEMDIALOG_H
#define ADDFAVITEMDIALOG_H

#include <QDialog>
#include "ui_AddFavItemDialog.h"
#include "Managers/vtkObjectsManager.h"

class AddFavItemDialog : public QDialog {
    Q_OBJECT

public:
    AddFavItemDialog(QWidget* parent = 0);
    AddFavItemDialog(ObjectManager* manager, QWidget* parent = 0);
    ~AddFavItemDialog();

    QComboBox* getGroupComboBox() {return this->ui.favGroupComboBox;}
    QLineEdit* getFavItemLineEdit() {return this->ui.favItemLineEdit;}
    QComboBox* getModelComboBox() {return this->ui.modelNameComboBox;}
    QComboBox* getObjComboBox() {return this->ui.objNameComboBox;}
    QPushButton* getOkBtn() {return this->ui.okButton;}

public slots:
    virtual void accept();
    void reloadObjComboBoxItems(int index);

private:
    int checkInfoValid();
    bool favItemExisted();
private:
    Ui::AddFavItemDialog ui;
    ObjectManager* objManager;

};

#endif // ADDFAVITEMDIALOG_H

#include <QRegExpValidator>
#include "AddFavGroupDialog.h"

AddFavGroupDialog::AddFavGroupDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QRegExp regExp("[A-Za-z0-9_]*");

    ui.GroupName->setValidator(new QRegExpValidator(regExp, this));
}

AddFavGroupDialog::~AddFavGroupDialog()
{

}

#include <QMessageBox>
#include "AddFavItemDialog.h"

typedef enum INVALID_TYPE{
    INVALID_TYPE_NOTHING,
    INVALID_TYPE_FAV_GROUP_EMPTY,
    INVALID_TYPE_FAV_ITEM_NAME_ERROR,
    INVALID_TYPE_MODEL_NAME_ERROR,
    INVALID_TYPE_OBJ_NAME_ERROR
}INVALID_TYPE;

AddFavItemDialog::AddFavItemDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

AddFavItemDialog::AddFavItemDialog(ObjectManager* manager, QWidget* parent)
    :QDialog(parent), objManager(manager)
{
    ui.setupUi(this);

    QRegExp regExp("[A-Za-z0-9_]*");

    ui.favItemLineEdit->setValidator(new QRegExpValidator(regExp, this));

    connect(this->ui.modelNameComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(reloadObjComboBoxItems(int)));
}

AddFavItemDialog::~AddFavItemDialog()
{

}

void AddFavItemDialog::accept()
{
    int checkResult = checkInfoValid();
    switch (checkInfoValid())
    {
    case INVALID_TYPE_NOTHING:
        done(Accepted);
        break;

    case INVALID_TYPE_FAV_GROUP_EMPTY:
        QMessageBox::information(this, tr("信息不完整"), tr("请选择收藏夹。"));
        break;

    case INVALID_TYPE_FAV_ITEM_NAME_ERROR:
        QMessageBox::information(this, tr("信息不完整"), tr("名称不合法。"));
        break;

    case INVALID_TYPE_MODEL_NAME_ERROR:
        QMessageBox::information(this, tr("信息不完整"), tr("请选择模型名称。"));
        break;

    case INVALID_TYPE_OBJ_NAME_ERROR:
        QMessageBox::information(this, tr("信息不完整"), tr("请选择对象名称。"));
        break;

    default:
        break;
    }

    return;
}

int AddFavItemDialog::checkInfoValid()
{
    if (this->ui.favGroupComboBox->currentText().isEmpty())
    {
        return INVALID_TYPE_FAV_GROUP_EMPTY;
    }
    else if (this->ui.favItemLineEdit->text().isEmpty() || favItemExisted())
    {
        return INVALID_TYPE_FAV_ITEM_NAME_ERROR;
    }
    else if (this->ui.modelNameComboBox->currentText().isEmpty())
    {
        return INVALID_TYPE_MODEL_NAME_ERROR;
    }
    else if (this->ui.objNameComboBox->currentText().isEmpty())
    {
        return INVALID_TYPE_OBJ_NAME_ERROR;
    }

    return INVALID_TYPE_NOTHING;
}

void AddFavItemDialog::reloadObjComboBoxItems(int index)
{
    QString currModelName = this->ui.modelNameComboBox->currentText();
    Model* pModel = objManager->findModel(currModelName);
    if (NULL == pModel)
    {
        QMessageBox::information(NULL, tr("错误"), tr("模型名错误。"));
        return;
    }

    this->ui.objNameComboBox->clear();

    for (unsigned int i = 0; i < pModel->getVecOfGeoObjs()->size(); ++i)
    {
        QString tmpObjName(pModel->getVecOfGeoObjs()->at(i).getName());
        this->ui.objNameComboBox->addItem(tmpObjName);
    }
}

bool AddFavItemDialog::favItemExisted()
{
    QString groupName(this->ui.favGroupComboBox->currentText());
    QString favItemName(this->ui.favItemLineEdit->text());
    if (NULL == this->objManager->findFavItem(groupName, favItemName))
    {
        return false;
    }
    else
    {
        return true;
    }
}

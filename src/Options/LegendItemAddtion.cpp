#include "LegendItemAddtion.h"
#include <QLineEdit>
#include <QMessageBox>

LegendItemAddtion::LegendItemAddtion(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connectSignalSlots();
}

LegendItemAddtion::~LegendItemAddtion()
{

}

void LegendItemAddtion::OnClickedOK()
{
    //name valaid
    if (ui.nameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("´íÎó"), tr("ÇëÌîÈëÃû³Æ"));
        return;
    }

    name = ui.nameLineEdit->text();
    rgb = QColor(ui.redSpinBox->value(),
                 ui.greenSpinBox->value(),
                 ui.blueSpinBox->value());
    description = ui.descriptionTextEdit->toPlainText();

    done(QDialog::Accepted);
    isAdd = true;
}

void LegendItemAddtion::OnBtnClickedCancle()
{
    done(QDialog::Rejected);
    isAdd = false;
}

void LegendItemAddtion::connectSignalSlots()
{
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnClickedOK()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBtnClickedCancle()));
}


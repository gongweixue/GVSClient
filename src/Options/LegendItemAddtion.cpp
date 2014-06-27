#include <QLineEdit>
#include <QMessageBox>
#include "LegendItemAddtion.h"

LegendItemAddtion::LegendItemAddtion(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connectSignalSlots();
    isAdd = false;
}

LegendItemAddtion::~LegendItemAddtion()
{

}

void LegendItemAddtion::OnClickedOK()
{
    //name valaid
    if (ui.nameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("����"), tr("����������"));
        return;
    }

    name = ui.nameLineEdit->text();
    rgb = QColor(ui.redSpinBox->value(),
                 ui.greenSpinBox->value(),
                 ui.blueSpinBox->value());
    description = ui.descriptionTextEdit->toPlainText();

    this->hide();
    isAdd = true;
}

void LegendItemAddtion::OnBtnClickedCancle()
{
    this->hide();
    isAdd = false;
}

void LegendItemAddtion::connectSignalSlots()
{
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnClickedOK()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBtnClickedCancle()));
}


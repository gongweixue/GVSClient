#include <QColorDialog>
#include <QLineEdit>
#include <QMessageBox>
#include "LegendItemAddtion.h"

LegendItemAddtion::LegendItemAddtion(QWidget* parent) : QDialog(parent) {
    ui.setupUi(this);
    connectSignalSlots();
    isAdd = false;
}

LegendItemAddtion::~LegendItemAddtion() {

}

void LegendItemAddtion::OnClickedOK() {
    //name valaid
    if (ui.nameLineEdit->text().isEmpty()) {
        QMessageBox::information(this, tr("´íÎó"), tr("ÇëÌîÈëÃû³Æ"));
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

void LegendItemAddtion::OnBtnClickedCancle() {
    this->hide();
    isAdd = false;
}

void LegendItemAddtion::connectSignalSlots() {
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnClickedOK()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBtnClickedCancle()));
    connect(ui.paletteButton, SIGNAL(clicked()), this, SLOT(OnPaletteBtnClicked()));
}

void LegendItemAddtion::OnPaletteBtnClicked() {
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid()) {
        ui.redSpinBox->setValue(color.red());
        ui.greenSpinBox->setValue(color.green());
        ui.blueSpinBox->setValue(color.blue());
    }
}


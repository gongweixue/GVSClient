#include "CubeAxesOption.h"

CubeAxesOption::CubeAxesOption(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

CubeAxesOption::CubeAxesOption(int cubeAxesOn, int xGridOn, int yGridOn, QWidget *parent)
{
    ui.setupUi(this);
    m_isCubeAxesOn = cubeAxesOn;
    m_isXGridOn = xGridOn;
    m_isYGridOn = yGridOn;
    this->initDialogCtrls();
}

CubeAxesOption::~CubeAxesOption()
{
}

void CubeAxesOption::initDialogCtrls()
{
    ui.CubeAxesCheckBoxCtrl->setChecked(m_isCubeAxesOn);
    ui.NSGridCheckboxCtrl->setChecked(m_isXGridOn);
    ui.NSGridCheckboxCtrl->setEnabled(m_isCubeAxesOn);
    ui.EWGridCheckBoxCtrl->setChecked(m_isYGridOn);
    ui.EWGridCheckBoxCtrl->setEnabled(m_isCubeAxesOn);
}

void CubeAxesOption::OnBnClickedOk()
{
    m_isCubeAxesOn = ui.CubeAxesCheckBoxCtrl->isChecked();
    m_isXGridOn = ui.NSGridCheckboxCtrl->isChecked();
    m_isYGridOn = ui.EWGridCheckBoxCtrl->isChecked();
}

void CubeAxesOption::OnClickCubeAxesOnCheckbox()
{
    ui.NSGridCheckboxCtrl->setEnabled(ui.CubeAxesCheckBoxCtrl->isChecked());
    ui.EWGridCheckBoxCtrl->setEnabled(ui.CubeAxesCheckBoxCtrl->isChecked());
}
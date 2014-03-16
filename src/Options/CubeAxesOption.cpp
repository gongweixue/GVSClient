#include "CubeAxesOption.h"


CubeAxesOption::CubeAxesOption(int cubeAxesOn, int xGridOn, int yGridOn, QWidget *parent)
{
    ui.setupUi(this);
    m_isCubeAxesOn = cubeAxesOn;
    m_isXGridOn = xGridOn;
    m_isYGridOn = yGridOn;
    this->initDialogCtrls();
    bindingSlots();
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
    this->close();
}

void CubeAxesOption::OnClickCubeAxesOnCheckbox()
{
    ui.NSGridCheckboxCtrl->setEnabled(ui.CubeAxesCheckBoxCtrl->isChecked());
    ui.EWGridCheckBoxCtrl->setEnabled(ui.CubeAxesCheckBoxCtrl->isChecked());
}

void CubeAxesOption::bindingSlots()
{
    connect(ui.CubeAxesCheckBoxCtrl, SIGNAL(clicked()), this, SLOT(OnClickCubeAxesOnCheckbox()));
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnBnClickedOk()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBnClickedCancle()));
}

void CubeAxesOption::OnBnClickedCancle()
{
    this->hide();
}

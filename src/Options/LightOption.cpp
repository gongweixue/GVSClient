#include "LightOption.h"

LightOption::LightOption(int camLightOn,
                         double IntencityOfCamLight,
                         int sceneLightOn,
                         double IntencityOfSceneLight,
                         double XYZ[3],
                         QWidget* pParent /*= 0*/ )
{
    ui.setupUi(this);

    isCamLightOn = camLightOn;
    isSceneLightOn = sceneLightOn;
    intencityCamLight = IntencityOfCamLight;
    intencitySceneLight = IntencityOfSceneLight;
    xSceneLight=XYZ[0];
    ySceneLight=XYZ[1];
    zSceneLight=XYZ[2];

    initDialogCtrls();
    bindingSlots();
}

LightOption::~LightOption()
{
}

void LightOption::initDialogCtrls()
{
    //cam light ctrls
    ui.CamLightCheckBoxCtrl->setChecked(isCamLightOn);
    ui.IntencityOfCamLightCtrl->setRange(0, 10);
    ui.IntencityOfCamLightCtrl->setValue(intencityCamLight * 10);
    ui.IntencityOfCamLightCtrl->setEnabled(isCamLightOn);

    //scene light check box ctrls
    ui.SceneLightCheckBoxCtrl->setChecked(isSceneLightOn);
    ui.IntencityOfSceneLightCtrl->setRange(0, 10);
    ui.IntencityOfSceneLightCtrl->setValue(intencitySceneLight * 10);
    ui.IntencityOfSceneLightCtrl->setEnabled(isSceneLightOn);

    ui.XofSceneLightCtrl->setRange(0,10);
    ui.XofSceneLightCtrl->setValue(xSceneLight * 10);
    ui.XofSceneLightCtrl->setEnabled(isSceneLightOn);

    ui.YofSceneLightCtrl->setRange(0,10);
    ui.YofSceneLightCtrl->setValue(ySceneLight * 10);
    ui.YofSceneLightCtrl->setEnabled(isSceneLightOn);

    ui.ZofSceneLightCtrl->setRange(0,10);
    ui.ZofSceneLightCtrl->setValue(zSceneLight * 10);
    ui.ZofSceneLightCtrl->setEnabled(isSceneLightOn);
}

void LightOption::OnClickCamLightCheckbox()
{
    ui.IntencityOfCamLightCtrl->setEnabled(ui.CamLightCheckBoxCtrl->isChecked());
}

void LightOption::OnClickSceneLightCheckbox()
{
    ui.IntencityOfSceneLightCtrl->setEnabled(ui.SceneLightCheckBoxCtrl->isChecked());
    ui.XofSceneLightCtrl->setEnabled(ui.SceneLightCheckBoxCtrl->isChecked());
    ui.YofSceneLightCtrl->setEnabled(ui.SceneLightCheckBoxCtrl->isChecked());
    ui.ZofSceneLightCtrl->setEnabled(ui.SceneLightCheckBoxCtrl->isChecked());
}

void LightOption::OnBnClickedOk()
{
    isCamLightOn = ui.CamLightCheckBoxCtrl->isChecked();
    if (isCamLightOn)
    {
        intencityCamLight = (double)(ui.IntencityOfCamLightCtrl->value()) / 10;
    }

    isSceneLightOn = ui.SceneLightCheckBoxCtrl->isChecked();
    if (isSceneLightOn)
    {
        intencitySceneLight=(double)(ui.IntencityOfSceneLightCtrl->value()) / 10;
        xSceneLight=(double)(ui.XofSceneLightCtrl->value()) / 10;
        ySceneLight=(double)(ui.YofSceneLightCtrl->value()) / 10;
        zSceneLight=(double)(ui.ZofSceneLightCtrl->value()) / 10;
    }
    this->close();
}

void LightOption::bindingSlots()
{
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnBnClickedOk()));
    connect(ui.CamLightCheckBoxCtrl,
            SIGNAL(clicked()),
            this,
            SLOT(OnClickCamLightCheckbox()));
    connect(ui.SceneLightCheckBoxCtrl,
            SIGNAL(clicked()),
            this,
            SLOT(OnClickSceneLightCheckbox()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBnClickCancel()));
}

void LightOption::OnBnClickCancel()
{
    this->hide();
}

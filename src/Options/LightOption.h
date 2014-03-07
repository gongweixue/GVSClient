#ifndef LIGHTOPTION_H
#define LIGHTOPTION_H

#include <QWidget>
#include "ui_LightOption.h"

class LightOption : public QWidget
{
    Q_OBJECT

public:
    LightOption(QWidget *parent = 0);
    LightOption(int camLightOn, double IntencityOfCamLight,
                int sceneLightOn, double IntencityOfSceneLight,
                double XYZ[3],QWidget* pParent = 0);
    ~LightOption();

    //slots
    void OnClickCamLightCheckbox();
    void OnClickSceneLightCheckbox();
    void OnBnClickedOk();

private:
    void initDialogCtrls();

private:
    Ui::LightOption ui;
    int isCamLightOn;
    int isSceneLightOn;
    double intencityCamLight;
    double intencitySceneLight;
    double xSceneLight;
    double ySceneLight;
    double zSceneLight;
};

#endif // LIGHTOPTION_H

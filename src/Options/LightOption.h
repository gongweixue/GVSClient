#ifndef LIGHTOPTION_H
#define LIGHTOPTION_H

#include <QWidget>
#include <QDialog>
#include "ui_LightOption.h"

class LightOption : public QDialog
{
    Q_OBJECT

public:
    LightOption(QWidget *parent = 0);
    LightOption(int camLightOn, double IntencityOfCamLight,
                int sceneLightOn, double IntencityOfSceneLight,
                double XYZ[3],QWidget* pParent = 0);
    ~LightOption();

public slots:
    void OnBnClickCancel();
    void OnBnClickedOk();
    void OnClickCamLightCheckbox();
    void OnClickSceneLightCheckbox();

private:
    void initDialogCtrls();
    void bindingSlots();
private:
    Ui::LightOption ui;

public:
    int isCamLightOn;
    int isSceneLightOn;
    double intencityCamLight;
    double intencitySceneLight;
    double xSceneLight;
    double ySceneLight;
    double zSceneLight;
};

#endif // LIGHTOPTION_H

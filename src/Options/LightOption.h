#ifndef LIGHT_OPTION_H
#define LIGHT_OPTION_H

#include <QDialog>
#include <QWidget>
#include "ui_LightOption.h"

class LightOption : public QDialog {
    Q_OBJECT

public:
    LightOption(int camLightOn, double IntencityOfCamLight,
                int sceneLightOn, double IntencityOfSceneLight,
                double XYZ[3], QWidget* pParent = 0);
    ~LightOption();

public slots:
    void OnBnClickCancel();
    void OnBnClickedOk();
    void OnClickCamLightCheckbox();
    void OnClickSceneLightCheckbox();

private:
    LightOption(QWidget* parent = 0);
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

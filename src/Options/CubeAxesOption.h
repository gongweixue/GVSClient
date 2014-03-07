#ifndef CUBEAXESOPTION_H
#define CUBEAXESOPTION_H

#include <QWidget>
#include "ui_CubeAxesOption.h"

class CubeAxesOption : public QWidget
{
    Q_OBJECT

public:
    CubeAxesOption(QWidget *parent = 0);
    CubeAxesOption(int cubeAxesOn, int xGridOn, int yGridOn, QWidget *parent = 0);
    ~CubeAxesOption();

private:
    //init the ui of the dialog
    void initDialogCtrls();
    //when click the ok btn, store the status of ctrls.
    void OnBnClickedOk();//slot
    //change the ctrl status when turn the checkbox on.
    void OnClickCubeAxesOnCheckbox();//slot

private:
    Ui::CubeAxesOption ui;
    int m_isCubeAxesOn;
    int m_isXGridOn;
    int m_isYGridOn;
};

#endif // CUBEAXESOPTION_H

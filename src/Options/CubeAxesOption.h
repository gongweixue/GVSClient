#ifndef CUBE_AXES_OPTION_H
#define CUBE_AXES_OPTION_H

#include <QWidget>
#include <QDialog>
#include "ui_CubeAxesOption.h"

class CubeAxesOption : public QDialog
{
    Q_OBJECT

public:
    CubeAxesOption(QWidget* parent = 0);
    CubeAxesOption(int cubeAxesOn, int xGridOn, int yGridOn, QWidget *parent = 0);
    ~CubeAxesOption();

private:
    //init the ui of the dialog
    void initDialogCtrls();
    void bindingSlots();

public slots:
    void OnBnClickedOk();
    void OnBnClickedCancle();
    //change the ctrl status when turn the checkbox on.
    void OnClickCubeAxesOnCheckbox();

private:
    Ui::CubeAxesOption ui;

public:
    int m_isCubeAxesOn;
    int m_isXGridOn;
    int m_isYGridOn;
};

#endif // CUBEAXESOPTION_H

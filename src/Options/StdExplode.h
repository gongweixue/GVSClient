#ifndef STDEXPLODE_H
#define STDEXPLODE_H

#include <QDialog>
#include "ui_StdExplode.h"

class StdExplode : public QDialog
{
    Q_OBJECT

public:
    StdExplode(QWidget *parent = 0);
    ~StdExplode();
    //slots
    void OnBnClickedRadioCrossExplode();
    void OnBnClickedRadioEvenExplode();
    void OnBnClickedOk();

private:
    void initDialogCtrls();

private:
    Ui::StdExplode ui;
    int m_RadioCrossExplodeChecked;
    int m_RadioEvenExplodeChecked;
    int m_evenRow;
    int m_evenCol;
    double m_gapRatio;
};

#endif // STDEXPLODE_H

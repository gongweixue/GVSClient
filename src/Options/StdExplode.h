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

public slots:
    void OnBnClickedRadioCrossExplode();
    void OnBnClickedRadioEvenExplode();
    void OnBnClickedOk();
    void OnBnClickedCancle();

private:
    void initDialogCtrls();
    void bindingSlots();
private:
    Ui::StdExplode ui;

public:
    int m_RadioCrossExplodeChecked;
    int m_RadioEvenExplodeChecked;
    int m_evenRow;
    int m_evenCol;
    double m_gapRatio;
};

#endif // STDEXPLODE_H

#include "StdExplode.h"

StdExplode::StdExplode(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    //init the member values
    m_RadioCrossExplodeChecked=0;
    m_RadioEvenExplodeChecked=0;
    m_evenRow=2;
    m_evenCol=2;
    m_gapRatio=1;

    initDialogCtrls();
}

StdExplode::~StdExplode()
{

}

void StdExplode::initDialogCtrls()
{
    ui.m_Radio_Cross_Explode->setChecked(0);
    ui.m_Radio_Even_Explode->setChecked(0);
    ui.m_EvenExplodeGapRatio->setCurrentIndex(ui.m_EvenExplodeGapRatio->count() - 1);
    ui.m_EvenExplodeRow->setEnabled(false);
    ui.m_EvenExplodeCol->setEnabled(false);
    ui.m_EvenExplodeGapRatio->setEnabled(false);
}

void StdExplode::OnBnClickedRadioCrossExplode()
{
    ui.m_EvenExplodeRow->setEnabled(false);
    ui.m_EvenExplodeCol->setEnabled(false);
    ui.m_EvenExplodeGapRatio->setEnabled(false);
    ui.m_Radio_Even_Explode->setChecked(0);
}

void StdExplode::OnBnClickedRadioEvenExplode()
{
    ui.m_EvenExplodeRow->setEnabled(true);
    ui.m_EvenExplodeCol->setEnabled(true);
    ui.m_EvenExplodeGapRatio->setEnabled(true);
    ui.m_Radio_Cross_Explode->setEnabled(0);
}

void StdExplode::OnBnClickedOk()
{
    m_RadioCrossExplodeChecked = ui.m_Radio_Cross_Explode->isChecked();
    m_RadioEvenExplodeChecked = ui.m_Radio_Even_Explode->isChecked();

    m_evenRow = ui.m_EvenExplodeRow->currentIndex() + 2;
    m_evenCol = ui.m_EvenExplodeCol->currentIndex() + 2;

    switch (ui.m_EvenExplodeGapRatio->currentIndex())
    {
    case 0:
        m_gapRatio=0.15;
        break;
    case 1:
        m_gapRatio=0.25;
        break;
    case 2:
        m_gapRatio=0.5;
        break;
    default:
        m_gapRatio=1;
    }
}

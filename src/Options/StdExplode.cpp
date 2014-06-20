#include "StdExplode.h"

StdExplode::StdExplode(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    //init the member values
    m_RadioCrossExplodeChecked = 0;
    m_RadioEvenExplodeChecked = 0;
    m_evenRow = 2;
    m_evenCol = 2;
    m_gapRatio = 1;

    initDialogCtrls();
    bindingSlots();
}

StdExplode::~StdExplode()
{
}

void StdExplode::initDialogCtrls()
{
    ui.m_Radio_Cross_Explode->setChecked(1);
    ui.m_Radio_Even_Explode->setChecked(0);
    ui.m_EvenExplodeGapRatio->setCurrentIndex(0);
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

    if ((!m_RadioCrossExplodeChecked) && (!m_RadioEvenExplodeChecked))
    {
        return;
    }

    m_evenRow = ui.m_EvenExplodeRow->currentIndex() + 2;
    m_evenCol = ui.m_EvenExplodeCol->currentIndex() + 2;

    switch (ui.m_EvenExplodeGapRatio->currentIndex())
    {
    case 0:
        m_gapRatio = 1;
        break;
    case 1:
        m_gapRatio = 0.5;
        break;
    case 2:
        m_gapRatio = 0.25;
        break;
    case 3:
        m_gapRatio = 0.15;
    default:
        m_gapRatio = 1;
    }
    this->close();
}

void StdExplode::bindingSlots()
{
    connect(ui.m_Radio_Cross_Explode, SIGNAL(clicked()),
            this, SLOT(OnBnClickedRadioCrossExplode()));
    connect(ui.m_Radio_Even_Explode, SIGNAL(clicked()),
            this, SLOT(OnBnClickedRadioEvenExplode()));
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OnBnClickedOk()));
    connect(ui.cancleButton, SIGNAL(clicked()), this, SLOT(OnBnClickedCancle()));
}

void StdExplode::OnBnClickedCancle()
{
    this->hide();
}

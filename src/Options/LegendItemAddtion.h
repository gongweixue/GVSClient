#ifndef LEGEND_ITEM_ADDTION_H
#define LEGEND_ITEM_ADDTION_H

#include <QDialog>
#include "ui_LegendItemAddtion.h"

class LegendItemAddtion : public QDialog
{
    Q_OBJECT

public:
    LegendItemAddtion(QWidget* parent = 0);
    ~LegendItemAddtion();
    bool getAddConfirm() {return isAdd;}

public slots:
    void OnClickedOK();
    void OnBtnClickedCancle();
    void setAddConfirm(bool b) {isAdd = b;}

public:
    QString name;
    QColor rgb;
    QString description;

private:
    void connectSignalSlots();

private:
    Ui::LegendItemAddtion ui;
    bool isAdd;
};

#endif // LEGENDITEMADDTION_H

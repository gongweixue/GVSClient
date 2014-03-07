#ifndef SCREENSHOTOPTION_H
#define SCREENSHOTOPTION_H

#include <QDialog>
#include "ui_ScreenShotOption.h"

class ScreenShotOption : public QDialog
{
    Q_OBJECT

public:
    ScreenShotOption(QWidget *parent = 0);
    ~ScreenShotOption();

private:
    Ui::ScreenShotOption ui;
};

#endif // SCREENSHOTOPTION_H

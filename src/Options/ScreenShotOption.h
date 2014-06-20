#ifndef SCREEN_SHOT_OPTION_H
#define SCREEN_SHOT_OPTION_H

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

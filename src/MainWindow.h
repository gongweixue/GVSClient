#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private:
    void initStatusBar();
    int loadLocalConfig();
    int loadNetwork();

private:
    Ui::MainWindowClass ui;
    QLabel* tipLabel;
    QLabel* secondLabel;
};

#endif // MAINWINDOW_H

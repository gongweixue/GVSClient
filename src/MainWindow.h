#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QSplitter>
#include <QtGui/QMainWindow>
#include <QTabWidget>
#include <QTreeView>
#include <QVTKWidget.h>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();
    void displayCube();

private:
    int loadLocalConfig();
    int loadNetwork();
    void initMembers();
    
    void destoryMembers();
    void initStatusBarMembers();
    void initMainAreaMembers();
    void destoryStatusBarMembers();
    void destoryMainAreaMembers();

    MainWindow(const MainWindow&); // Not implemented.
    void operator=(const MainWindow&); // Not implemented.

    
private:
    Ui::MainWindowClass ui;
    QLabel* statusBartipLabel;
    QLabel* statusBartipLabel2;
    QSplitter* splitterMain;
    QTabWidget* leftTab;
    QTreeView* treeTab1;
    QTreeView* treeTab2;
    QVTKWidget* qvtkWidget;
    
};

#endif // MAINWINDOW_H

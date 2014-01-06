#include <QMessageBox>
#include <QSplashScreen>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    //show startup image while preparing.
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/Resources/startup.jpg"));
    splash->show();

    //show some sentence when load widget, config and network.
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("¼ÓÔØ´°¿Ú¿Ø¼þ"), topRight, Qt::red);
    ui.setupUi(this);
    initStatusBar();
    splash->showMessage(QObject::tr("¼ÓÔØ±¾µØÅäÖÃ"), topRight, Qt::red);
    if (loadLocalConfig() != 0) {
        std::printf("±¾µØÅäÖÃ¼ÓÔØÊ§°Ü");
        abort();
    }

    splash->showMessage(QObject::tr("ÉèÖÃÍøÂç"), topRight, Qt::red);
    if (loadNetwork() != 0) {
        QMessageBox::information(this, "ÍøÂç³õÊ¼»¯´íÎó", NULL, QMessageBox::Yes);
    }
}

MainWindow::~MainWindow()
{
    delete tipLabel;
    delete secondLabel;
}

void MainWindow::initStatusBar()
{
    QStatusBar* statBar = this->statusBar();

    tipLabel = new QLabel(QObject::tr("ready"));
    tipLabel->setAlignment(Qt::AlignHCenter);
    tipLabel->setMinimumSize(tipLabel->sizeHint());

    secondLabel = new QLabel(QObject::tr("second tip"));
    secondLabel->setIndent(3);

    statBar->addWidget(tipLabel);
    statBar->addWidget(secondLabel);
    Sleep(1000);
}

int MainWindow::loadLocalConfig()
{
    //TODO: load some config and return the status code:
    //0 is ok and other value means there are some errors, then abort.
    Sleep(1000);
    return 0;
}

int MainWindow::loadNetwork()
{
    //TODO: load some network info and return a code.
    //0 is ok and other value is error, and give a tip to user.
    Sleep(1000);
    return 0;
}

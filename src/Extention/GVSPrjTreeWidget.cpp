#include <QContextMenuEvent>
#include <QMenu>
#include <QSplitter>
#include <QVTKWidget.h>
#include "vtkRenderWindow.h"
#include "GVSDoc.h"
#include "GVSPrjTreeWidget.h"
#include "MainWindow.h"

GVSPrjTreeWidget::GVSPrjTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    this->popMenu = new QMenu(this);
    this->actChangeObjColor = new QAction(tr("更改颜色"), this);

    connect(actChangeObjColor, SIGNAL(triggered()), this, SLOT(OnChangeObjColor()));
}

GVSPrjTreeWidget::~GVSPrjTreeWidget()
{
    delete this->actChangeObjColor;
    delete this->popMenu;
}

void GVSPrjTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QTreeWidgetItem* item = NULL;
    QPoint pos = event->pos();
    item = this->itemAt(pos);

    //add some conditions to judge if the item is obj or model.
    if (item != NULL && item->childCount() == 0)
    {
        this->popMenu->clear();
        this->popMenu->addAction(actChangeObjColor);
        
        this->popMenu->exec(QCursor::pos());
        event->accept();
    }
}
#include <QMessageBox>
void GVSPrjTreeWidget::OnChangeObjColor()
{
    QString objName = this->currentItem()->text(0);
    QString modelName = this->currentItem()->parent()->text(0);

    QTabWidget* prjExplorer = (QTabWidget*)this->parent();
    QSplitter* mainSplitter = (QSplitter*)prjExplorer->parent();
    MainWindow* mainWindow = (MainWindow*)mainSplitter->parent();
    ObjectManager* objManager = mainWindow->getDocument()->GetObjManager();
    
    if (objManager->setObjColorByName(modelName, objName) == false)
    {
        QMessageBox::information(NULL, tr("操作异常"),
                                 tr("找不到对象") + modelName + "/" + objName);

        return;
    }

    if (mainWindow->getQVTKWidget()->GetRenderWindow())
    {
        mainWindow->getQVTKWidget()->GetRenderWindow()->Render();
    }
}


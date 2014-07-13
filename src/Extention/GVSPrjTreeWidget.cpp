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
    this->actChangeObjColor = new QAction(tr("¸ü¸ÄÑÕÉ«"), this);

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

    emit objColorClicked(modelName, objName);
}


#include <QContextMenuEvent>
#include <QMenu>
#include <QSplitter>
#include <QVTKWidget.h>
#include "vtkRenderWindow.h"
#include "GVSDoc.h"
#include "GVSPrjTreeWidget.h"
#include "GVSPrjTreeWidgetItem.h"
#include "MainWindow.h"

GVSPrjTreeWidget::GVSPrjTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    this->popMenu = new QMenu(this);

    this->actionChangeObjColor = new QAction(tr("更改颜色"), this);
    connect(actionChangeObjColor, SIGNAL(triggered()), this, SLOT(OnChangeObjColor()));

    this->actionAddFavGroup = new QAction(tr("新建收藏群组"), this);
    connect(actionAddFavGroup, SIGNAL(triggered()), this, SLOT(OnAddFavGroup()));
}

GVSPrjTreeWidget::~GVSPrjTreeWidget()
{
    delete this->actionChangeObjColor;
    delete this->popMenu;
}

void GVSPrjTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    GVSPrjTreeWidgetItem* item = NULL;
    QPoint pos = event->pos();
    item = dynamic_cast<GVSPrjTreeWidgetItem*>(this->itemAt(pos));

    switch (item->getType())
    {
    case PRJ_TREE_ITEM_TYPE_OBJ:
        this->popMenu->clear();
        this->popMenu->addAction(actionChangeObjColor);
        this->popMenu->exec(QCursor::pos());
        event->accept();
        break;

    case PRJ_TREE_ITEM_TYPE_FAV_ROOT:
        this->popMenu->clear();
        this->popMenu->addAction(actionAddFavGroup);
        this->popMenu->exec(QCursor::pos());
        break;

    case PRJ_TREE_ITEM_TYPE_FAV_GROUP:
        this->popMenu->clear();
        this->popMenu->addAction(actionAddFavGroup);
        this->popMenu->exec(QCursor::pos());
        break;

    default:
        break;
    }

    return;
}

void GVSPrjTreeWidget::OnChangeObjColor()
{
    QString objName = this->currentItem()->text(0);
    QString modelName = this->currentItem()->parent()->text(0);

    emit sigChangeObjColor(modelName, objName);
}

void GVSPrjTreeWidget::OnAddFavGroup()
{
    emit sigAddFavGroup();
}


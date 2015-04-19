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

    this->actionAddFavItem = new QAction(tr("新建收藏项"), this);
    connect(actionAddFavItem, SIGNAL(triggered()), this, SLOT(OnAddFavItem()));

    this->actionRenameGroup = new QAction(tr("重命名"), this);
    connect(actionRenameGroup, SIGNAL(triggered()), this, SLOT(OnRenameGroup()));

    this->actionRemoveGroup = new QAction(tr("移除收藏群组"), this);
    connect(actionRemoveGroup, SIGNAL(triggered()), this, SLOT(OnRemoveGroup()));

    this->actionRemoveFavItem = new QAction(tr("删除收藏项"), this);
    connect(actionRemoveFavItem, SIGNAL(triggered()), this, SLOT(OnRemoveFavItem()));

    this->actionEditFavItem = new QAction(tr("编辑收藏项"), this);
    connect(actionEditFavItem, SIGNAL(triggered()), this, SLOT(OnEditFavItem()));

    this->actionRemoveObj = new QAction(tr("移除地层对象"), this);
    connect(actionRemoveObj, SIGNAL(triggered()), this, SLOT(OnRemoveObj()));
}

GVSPrjTreeWidget::~GVSPrjTreeWidget()
{
    delete this->actionChangeObjColor;
    delete this->actionAddFavGroup;
    delete this->actionAddFavItem;
    delete this->actionRenameGroup;
    delete this->actionRemoveGroup;
    delete this->actionRemoveFavItem;
    delete this->actionEditFavItem;
    delete this->actionRemoveObj;
    delete this->popMenu;
}

void GVSPrjTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
    GVSPrjTreeWidgetItem* item = NULL;
    QPoint pos = event->pos();
    if (this->itemAt(pos) == NULL)
    {
        return;
    }
    item = dynamic_cast<GVSPrjTreeWidgetItem*>(this->itemAt(pos));

    switch (item->getType())
    {
    case PRJ_TREE_ITEM_TYPE_OBJ:
        this->popMenu->clear();
        /*this->popMenu->addAction(actionChangeObjColor);*/
        this->popMenu->addAction(actionRemoveObj);
        this->popMenu->exec(QCursor::pos());
        event->accept();
        break;

    case PRJ_TREE_ITEM_TYPE_FAV_ROOT:
        this->popMenu->clear();
        this->popMenu->addAction(actionAddFavGroup);
        this->popMenu->addAction(actionAddFavItem);
        this->popMenu->exec(QCursor::pos());
        event->accept();
        break;

    case PRJ_TREE_ITEM_TYPE_FAV_GROUP:
        this->popMenu->clear();
        this->popMenu->addAction(actionAddFavGroup);
        this->popMenu->addAction(actionAddFavItem);
        this->popMenu->addAction(actionRenameGroup);
        this->popMenu->addAction(actionRemoveGroup);
        this->popMenu->exec(QCursor::pos());
        event->accept();
        break;

    case PRJ_TREE_ITEM_TYPE_FAV_ITEM:
        this->popMenu->clear();
        this->popMenu->addAction(actionEditFavItem);
        this->popMenu->addAction(actionRemoveFavItem);
        this->popMenu->exec(QCursor::pos());
        event->accept();
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

void GVSPrjTreeWidget::OnAddFavItem()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigAddFavItem(*currentItem);
}

void GVSPrjTreeWidget::OnRenameGroup()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigRenameGroup(*currentItem);
}

void GVSPrjTreeWidget::OnRemoveGroup()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigRemoveGroup(*currentItem);
}

void GVSPrjTreeWidget::OnRemoveFavItem()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigRemoveFavItem(*currentItem);
}

void GVSPrjTreeWidget::OnEditFavItem()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigEditFavItem(*currentItem);
}

void GVSPrjTreeWidget::OnRemoveObj()
{
    GVSPrjTreeWidgetItem* currentItem =
            dynamic_cast<GVSPrjTreeWidgetItem*> (this->currentItem());

    emit sigRemoveObj(*currentItem);
}


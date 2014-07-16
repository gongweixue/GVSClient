#include "GVSPrjTreeWidgetItem.h"

GVSPrjTreeWidgetItem::GVSPrjTreeWidgetItem(QTreeWidget* parent, GVS_TREE_ITEM_TYPE ty)
    : QTreeWidgetItem(parent), itemType(ty)
{

}

GVSPrjTreeWidgetItem::GVSPrjTreeWidgetItem(QTreeWidgetItem* parent, GVS_TREE_ITEM_TYPE ty)
    : QTreeWidgetItem(parent), itemType(ty)
{

}


GVSPrjTreeWidgetItem::~GVSPrjTreeWidgetItem()
{

}

#ifndef GVSPRJTREEWIDGETITEM_H
#define GVSPRJTREEWIDGETITEM_H

#include <QTreeWidgetItem>

typedef enum GVS_TREE_ITEM_TYPE {
    PRJ_TREE_ITEM_TYPE_MODEL,
    PRJ_TREE_ITEM_TYPE_OBJ,
    PRJ_TREE_ITEM_TYPE_FAV_ROOT,
    PRJ_TREE_ITEM_TYPE_FAV_GROUP,
    PRJ_TREE_ITEM_TYPE_FAV_ITEM,
    PRJ_TREE_ITEM_TYPE_INVALID
} GVS_TREE_ITEM_TYPE;

class GVSPrjTreeWidgetItem : public QWidget, public QTreeWidgetItem
{
    Q_OBJECT

public:
    explicit GVSPrjTreeWidgetItem(QTreeWidget* parent, GVS_TREE_ITEM_TYPE ty);
    explicit GVSPrjTreeWidgetItem(QTreeWidgetItem* parent, GVS_TREE_ITEM_TYPE ty);
    ~GVSPrjTreeWidgetItem();

    QTreeWidgetItem* parent() const
    {
        return QTreeWidgetItem::parent();
    }

    GVS_TREE_ITEM_TYPE getType() {return this->itemType;}

protected:
    GVSPrjTreeWidgetItem(QWidget* parent);

private:
    const GVS_TREE_ITEM_TYPE itemType;//Can not be changed!
};

#endif // GVSPRJTREEWIDGETITEM_H

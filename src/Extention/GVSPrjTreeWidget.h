#ifndef GVS_PRJ_TREE_WIDGET_H
#define GVS_PRJ_TREE_WIDGET_H

#include <QTreeWidget>

class GVSPrjTreeWidgetItem;

class GVSPrjTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    GVSPrjTreeWidget(QWidget* parent);
    ~GVSPrjTreeWidget();

signals:
    void sigChangeObjColor(QString& modelName, QString& objName);
    void sigAddFavGroup();
    void sigAddFavItem(GVSPrjTreeWidgetItem& currentItem);
    void sigRenameGroup(GVSPrjTreeWidgetItem& currentItem);
    void sigRemoveGroup(GVSPrjTreeWidgetItem& currentItem);
    void sigRemoveFavItem(GVSPrjTreeWidgetItem& currentItem);
    void sigEditFavItem(GVSPrjTreeWidgetItem& currentItem);
    void sigRemoveObj(GVSPrjTreeWidgetItem& currentItem);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void OnChangeObjColor();
    void OnAddFavGroup();
    void OnAddFavItem();
    void OnRenameGroup();
    void OnRemoveGroup();
    void OnRemoveFavItem();
    void OnEditFavItem();
    void OnRemoveObj();

private:
    QMenu* popMenu;
    QAction* actionAddFavGroup;
    QAction* actionAddFavItem;
    QAction* actionRenameGroup;
    QAction* actionRemoveGroup;
    QAction* actionRemoveFavItem;
    QAction* actionEditFavItem;

    QAction* actionChangeObjColor;
    QAction* actionRemoveObj;
};

#endif // GVSPRJTREEWIDGET_H

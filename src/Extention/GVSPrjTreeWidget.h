#ifndef GVS_PRJ_TREE_WIDGET_H
#define GVS_PRJ_TREE_WIDGET_H

#include <QTreeWidget>

class GVSPrjTreeWidgetItem;

class GVSPrjTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    GVSPrjTreeWidget(QWidget *parent);
    ~GVSPrjTreeWidget();

signals:
    void sigChangeObjColor(QString& modelName, QString& objName);
    void sigAddFavGroup();
    void sigAddFavItem(GVSPrjTreeWidgetItem& currentItem);
    void sigRenameGroup(GVSPrjTreeWidgetItem& currentItem);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void OnChangeObjColor();
    void OnAddFavGroup();
    void OnAddFavItem();
    void OnRenameGroup();
private:
    QMenu* popMenu;
    QAction* actionChangeObjColor;
    QAction* actionAddFavGroup;
    QAction* actionAddFavItem;
    QAction* actionRenameGroup;
};

#endif // GVSPRJTREEWIDGET_H

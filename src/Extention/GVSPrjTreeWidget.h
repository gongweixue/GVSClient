#ifndef GVS_PRJ_TREE_WIDGET_H
#define GVS_PRJ_TREE_WIDGET_H

#include <QTreeWidget>

class GVSPrjTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    GVSPrjTreeWidget(QWidget *parent);
    ~GVSPrjTreeWidget();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void OnChangeObjColor();

private:
    QMenu* popMenu;
    QAction* actChangeObjColor;
};

#endif // GVSPRJTREEWIDGET_H

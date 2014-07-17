#ifndef GVS_PRJ_TREE_WIDGET_H
#define GVS_PRJ_TREE_WIDGET_H

#include <QTreeWidget>

class GVSPrjTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    GVSPrjTreeWidget(QWidget *parent);
    ~GVSPrjTreeWidget();

signals:
    void sigChangeObjColor(QString& modelName, QString& objName);
    void sigAddFavGroup();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void OnChangeObjColor();
    void OnAddFavGroup();
private:
    QMenu* popMenu;
    QAction* actionChangeObjColor;
    QAction* actionAddFavGroup;
};

#endif // GVSPRJTREEWIDGET_H

#ifndef COLORLEGENDMANAGER_H
#define COLORLEGENDMANAGER_H

#include <QObject>
#include <QDockWidget>


class ColorLegendManager : public QObject
{
    Q_OBJECT

public:
    ColorLegendManager(QObject *parent);
    ColorLegendManager(QDockWidget* pLegend, QObject* parent = 0);
    ~ColorLegendManager();

private:
    
};

#endif // COLORLEGENDMANAGER_H

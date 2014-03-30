#ifndef COLORLEGENDEDITOR_H
#define COLORLEGENDEDITOR_H

#include <QDialog>
#include <vector>
#include "ui_ColorLegendEditor.h"
#include "Managers/ColorLegendManager.h"

class ColorLegendEditor : public QDialog
{
    Q_OBJECT

public:
    ColorLegendEditor(ColorLegendManager* manager, QWidget *parent = 0);
    ~ColorLegendEditor();

private:
    ColorLegendEditor(QWidget *parent = 0);
    void initUpdate();
    void fillListWidget();
    void connectSignalSlots();

public slots:
    void OnAddItem();
    void OnBtnClickedClose();

private:
    Ui::ColorLegendEditor ui;
    ColorLegendManager* m_manager;
    vector<QListWidgetItem> vecOfItems;
    LegendRecord recordToInsert;

};

#endif // COLORLEGENDEDITOR_H

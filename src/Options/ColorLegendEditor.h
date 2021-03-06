#ifndef COLOR_LEGEND_EDITOR_H
#define COLOR_LEGEND_EDITOR_H

#include <vector>
#include <QDialog>
#include "ui_ColorLegendEditor.h"
#include "Managers/ColorLegendManager.h"

class ColorLegendEditor : public QDialog {
    Q_OBJECT

public:
    ColorLegendEditor(ColorLegendManager* manager, QWidget* parent = 0);
    ~ColorLegendEditor();

private:
    ColorLegendEditor(QWidget* parent = 0);
    void initUpdate();
    void fillListWidget();
    void connectSignalSlots();

public slots:
    void OnAddItem();
    void OnBtnClickedClose();
    void OnEditItem();
    void OnDelItem();
private:
    Ui::ColorLegendEditor ui;
    ColorLegendManager* m_manager;
    vector<QListWidgetItem> vecOfItems;

};

#endif // COLOR_LEGEND_EDITOR_H

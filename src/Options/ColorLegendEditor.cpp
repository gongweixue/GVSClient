#include "ColorLegendEditor.h"
#include "Options/LegendItemAddtion.h"
#include <QMessageBox>

ColorLegendEditor::ColorLegendEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    throw std::exception("ColorLegendEditor's Default constructor is not implement.");
}

ColorLegendEditor::ColorLegendEditor(ColorLegendManager* manager, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_manager = manager;
    connectSignalSlots();
    recordToInsert.name.clear();
    initUpdate();
}

ColorLegendEditor::~ColorLegendEditor()
{

}

void ColorLegendEditor::initUpdate()
{
    fillListWidget();
    m_manager->initOrUpdateLegend(m_manager->getFilePath());
}

void ColorLegendEditor::fillListWidget() {
    vector<LegendRecord>* vecRecord = m_manager->getVecPtrOfRecord();
    if (vecRecord->empty())
    {
        return;
    }
    else
    {
        if (!recordToInsert.name.empty())
        {
            vecRecord->push_back(recordToInsert);
        }
    }
    vecOfItems.clear();
    //generate list items and store
    vector<LegendRecord>::const_iterator iterRecord;
    for (iterRecord = vecRecord->cbegin(); iterRecord != vecRecord->cend(); iterRecord++)
    {
        //item text
        QString itemName(tr(iterRecord->name.c_str()));
        QListWidgetItem item(itemName);
        //item color
        item.setBackgroundColor(iterRecord->rgb);
        QColor textColor(255 - iterRecord->rgb.red(),
                         255 - iterRecord->rgb.green(),
                         255 - iterRecord->rgb.blue());
        item.setTextColor(textColor);

        //store item
        vecOfItems.push_back(item);
    }
    //add into list
    vector<QListWidgetItem>::iterator iterItem;
    for (iterItem = vecOfItems.begin(); iterItem != vecOfItems.end(); iterItem++)
    {
        ui.listWidget->addItem(&(*iterItem));
    }
}

void ColorLegendEditor::connectSignalSlots()
{
    connect(ui.addButton, SIGNAL(clicked()), this, SLOT(OnAddItem()));
    connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(OnBtnClickedClose()));
}

void ColorLegendEditor::OnAddItem()
{
//     QMessageBox::information(NULL, "log", "ColorLegendEditor::OnAddItem()");
//     throw std::exception("This point always been invoked twice, should be fixed!!!!");
//     //If removing the belowing real operation works? Should been tested.!!!!!!

    LegendItemAddtion itemAddDialog(this);
    itemAddDialog.exec();
    if (!m_manager->getFilePath().empty() && itemAddDialog.getAddConfirm())
    {
        //insert into file
        bool insertOk = m_manager->insertItemToFile(
            itemAddDialog.name.toStdString(),
            itemAddDialog.rgb,
            itemAddDialog.description.toStdString());
        if (insertOk)
        {
            recordToInsert.name = itemAddDialog.name.toStdString();
            recordToInsert.rgb = itemAddDialog.rgb;
            recordToInsert.description = itemAddDialog.description.toStdString();

            initUpdate();
        }
    }
}

void ColorLegendEditor::OnBtnClickedClose()
{
    this->hide();
}


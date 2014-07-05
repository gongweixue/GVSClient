#include <QMessageBox>
#include "ColorLegendEditor.h"
#include "Options/LegendItemAddtion.h"


ColorLegendEditor::ColorLegendEditor(QWidget* parent)
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
    initUpdate();
}

ColorLegendEditor::~ColorLegendEditor()
{

}

void ColorLegendEditor::initUpdate()
{
    m_manager->initOrUpdateLegend(m_manager->getFilePath());
    fillListWidget();
}

void ColorLegendEditor::fillListWidget()
{
    vector<LegendRecord>* vecRecord = m_manager->getVecPtrOfRecord();
    if (vecRecord->empty())
    {
        return;
    }

    vecOfItems.clear();

    //first, insert an blank item to avoid that the current item is null.
    vecOfItems.push_back(QListWidgetItem(""));

    //generate list items and store
    vector<LegendRecord>::const_iterator iterRecord;
    for (iterRecord = vecRecord->cbegin(); iterRecord != vecRecord->cend(); iterRecord++)
    {
        //item text
        QString itemName(tr(iterRecord->name.c_str()));
        QListWidgetItem item(itemName);
        //item color
        item.setBackgroundColor(iterRecord->rgb);
        QColor textColor((128 + iterRecord->rgb.red()) % 256,
                         (128 + iterRecord->rgb.green()) % 256,
                         (128 - iterRecord->rgb.blue()) % 256);
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
    ui.listWidget->setCurrentItem(ui.listWidget->item(0));
}

void ColorLegendEditor::connectSignalSlots()
{
    connect(ui.addButton, SIGNAL(clicked()), this, SLOT(OnAddItem()));
    connect(ui.editButton, SIGNAL(clicked()), this, SLOT(OnEditItem()));
    connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(OnBtnClickedClose()));
}

void ColorLegendEditor::OnAddItem()
{
    LegendItemAddtion itemAddDialog(this);
    itemAddDialog.setWindowTitle(tr("添加图例"));
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
            initUpdate();
        }
    }

    ui.listWidget->setCurrentRow(0);
}

void ColorLegendEditor::OnEditItem()
{
    if (0 == ui.listWidget->count())
    {
        return;
    }
    //get item name
    string clickedItemName = ui.listWidget->currentItem()->text().toStdString();

    vector<LegendRecord>* vecRecord = m_manager->getVecPtrOfRecord();
    if (vecRecord->empty())
    {
        return;
    }
    //search item by name.
    vector<LegendRecord>::iterator iterRecord;
    for (iterRecord = vecRecord->begin(); iterRecord != vecRecord->end(); iterRecord++)
    {
        if (0 == iterRecord->name.compare(clickedItemName))
        {
            break;
        }
    }
    //if not found the item info in vector.
    if (iterRecord == vecRecord->end())
    {
        QMessageBox::information(this, "错误项", "没有指定项");
        return;
    }

    LegendItemAddtion itemEditDialog(this);
    itemEditDialog.setWindowTitle(tr("编辑图例"));
    itemEditDialog.getUI()->labelTip->setText(tr("请编辑图例信息"));
    itemEditDialog.getUI()->okButton->setText(tr("更新"));
    itemEditDialog.getUI()->nameLineEdit->setText(iterRecord->name.c_str());
    itemEditDialog.getUI()->nameLineEdit->setReadOnly(true);
    itemEditDialog.getUI()->redSpinBox->setValue(iterRecord->rgb.red());
    itemEditDialog.getUI()->greenSpinBox->setValue(iterRecord->rgb.green());
    itemEditDialog.getUI()->blueSpinBox->setValue(iterRecord->rgb.blue());
    itemEditDialog.getUI()->descriptionTextEdit->setPlainText(iterRecord->description.c_str());
    itemEditDialog.exec();
    
    if (!m_manager->getFilePath().empty() && itemEditDialog.getAddConfirm())
    {
        //if any data update
        if (0 != itemEditDialog.name.compare(iterRecord->name.c_str()) ||
            0 != itemEditDialog.description.compare(iterRecord->description.c_str()) ||
            iterRecord->rgb.red() != itemEditDialog.rgb.red() ||
            iterRecord->rgb.green() != itemEditDialog.rgb.green() ||
            iterRecord->rgb.blue() != itemEditDialog.rgb.blue())
        {
            //edit item
            bool editOk = m_manager->editItemInFile(
                itemEditDialog.name.toStdString(),
                itemEditDialog.rgb,
                itemEditDialog.description.toStdString());
            if (editOk)
            {
                initUpdate();
            }
        }
    }

    ui.listWidget->setCurrentRow(0);
    return;
}

void ColorLegendEditor::OnBtnClickedClose()
{
    this->hide();
}


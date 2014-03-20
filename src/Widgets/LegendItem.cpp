#include "LegendItem.h"
#include <QLabel>

LegendItem::LegendItem(LegendRecord rcrd, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    record = rcrd;

    //making a pic
    QImage img(60, 30, QImage::Format_RGB32);
    img.fill(record.rgb);
    QLabel* imgLabel = findChild<QLabel*>(tr("imgLabel"));
    imgLabel->setPixmap(QPixmap::fromImage(img));
    imgLabel->setToolTip(record.description.c_str());

    QLabel* nameLabel = findChild<QLabel*>(tr("nameLabel"));
    nameLabel->setText(record.name.c_str());
    nameLabel->setToolTip(record.description.c_str());
}

LegendItem::~LegendItem()
{

}

#include <QColorDialog>
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkObjectsManager.h"
#include "Utils/GVSUtils.h"
#include "Utils/vtkTotallyInclude.h"


ObjectManager::ObjectManager(void)
{
    this->objTreeModified = false;
    this->favTreeModified = false;
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for (; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->getVecOfGeoObjs()->clear();
    }
    treeOfGeoObjs.clear();

    this->pProgressDlg = new QProgressDialog();
}

ObjectManager::~ObjectManager(void)
{
    DelAllRdrsInObjTree();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for (; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->getVecOfGeoObjs()->clear();
    }
    treeOfGeoObjs.clear();

    if (pProgressDlg)
    {
        delete pProgressDlg;
    }
}

void ObjectManager::LoadDataForReadersInTree()
{
    ////progress to show the process of loading objects.
    pProgressDlg->setWindowModality(Qt::ApplicationModal);
    pProgressDlg->setMinimumDuration(5);
    pProgressDlg->setWindowTitle(tr("请稍候"));
    pProgressDlg->setLabelText(QString());
    pProgressDlg->setCancelButtonText(tr("取消"));
    pProgressDlg->setRange(0,getNumOfObjsInTree());
    pProgressDlg->setCancelButton(0);
    pProgressDlg->setWindowFlags(Qt::FramelessWindowHint);
    pProgressDlg->setMaximumWidth(400);
    pProgressDlg->setMinimumWidth(400);

    //update every record's reader.
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->getVecOfGeoObjs()->begin();
        for ( ; obj_iter != model_iter->getVecOfGeoObjs()->end(); obj_iter++)
        {
            QString loadingTip("正在加载模型：                \n");
            loadingTip.append(model_iter->getModelName() + "/" + obj_iter->getName());
            pProgressDlg->setLabelText(loadingTip);

            obj_iter->reader->Update();
            pProgressDlg->setValue(pProgressDlg->value() + 1);
            /*ReaderUpdater updater(this, obj_iter->reader);
            updater.start();
            updater.wait();*/
        }
    }

    pProgressDlg->hide();
    computeObjTreeBound();
}

void ObjectManager::DelAllRdrsInObjTree()
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->getVecOfGeoObjs()->begin();
        for ( ; obj_iter != model_iter->getVecOfGeoObjs()->end(); obj_iter++)
        {
            DeleteVTKPointer(obj_iter->reader);
        }
        model_iter->getVecOfGeoObjs()->clear();
    }
    computeObjTreeBound();
}

void ObjectManager::ClearObjTree()
{
    DelAllRdrsInObjTree();
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->getVecOfGeoObjs()->clear();
    }
    treeOfGeoObjs.clear();
}

double* ObjectManager::computeObjTreeBound()
{
    for (int i = 0; i < 6; ++i)
    {
        m_bounds[i] = .0;
    }
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->getVecOfGeoObjs()->begin();
        for ( ; obj_iter != model_iter->getVecOfGeoObjs()->end(); obj_iter++)
        {
            double tmpBnds[6];
            obj_iter->reader->GetOutput()->GetBounds(tmpBnds);

            m_bounds[0] = (tmpBnds[0] < m_bounds[0])? tmpBnds[0] : m_bounds[0];
            m_bounds[1] = (tmpBnds[1] > m_bounds[1])? tmpBnds[1] : m_bounds[1];
            m_bounds[2] = (tmpBnds[2] < m_bounds[2])? tmpBnds[2] : m_bounds[2];
            m_bounds[3] = (tmpBnds[3] > m_bounds[3])? tmpBnds[3] : m_bounds[3];
            m_bounds[4] = (tmpBnds[4] < m_bounds[4])? tmpBnds[4] : m_bounds[4];
            m_bounds[5] = (tmpBnds[5] > m_bounds[5])? tmpBnds[5] : m_bounds[5];
        }
    }

    return m_bounds;
}

int ObjectManager::getNumOfObjsInTree()
{
    int ret = 0;
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        ret += model_iter->getVecOfGeoObjs()->size();
    }

    return ret;
}

QMutex progressValueMutex;
void ObjectManager::OnObjUpdateFinished()
{
    progressValueMutex.lock();
    this->pProgressDlg->setValue(this->pProgressDlg->value() + 1);
    progressValueMutex.unlock();
}

GeoObject* ObjectManager::findObj(QString modelName, QString objName)
{
    //time complexity: m + n
    vector<Model>::iterator modelIter = treeOfGeoObjs.begin();
    for ( ; modelIter < treeOfGeoObjs.end(); modelIter++)
    {
        if (0 == modelName.compare(modelIter->getModelName()))
        {
            vector<GeoObject>::iterator objIter = modelIter->getVecOfGeoObjs()->begin();
            for ( ; objIter < modelIter->getVecOfGeoObjs()->end(); objIter++)
            {
                if (0 == objName.compare(objIter->getName()))
                {
                    return &(*objIter);
                }
            }
        }
    }

    return NULL;
}

bool ObjectManager::setModelModified(QString modelName, bool hasModified)
{
    vector<Model>::iterator modelIter = treeOfGeoObjs.begin();
    for ( ; modelIter < treeOfGeoObjs.end(); modelIter++)
    {
        if (0 == modelName.compare(modelIter->getModelName()))
        {
            modelIter->setModified(hasModified);
            return true;
        }
    }

    return false;
}

bool ObjectManager::setObjVis(QString modelName, QString objName, bool vis)
{
    GeoObject* obj = findObj(modelName, objName);
    if (obj)
    {
        obj->setVisibility(vis);
        obj->setModified(true);
        setModelModified(modelName, true);
        setObjTreeModified(true);

        return true;
    }
    return false;
}

bool ObjectManager::setObjColor(QString modelName, QString objName, int r, int g, int b)
{
    GeoObject* obj = findObj(modelName, objName);
    if (obj)
    {
        //get the color of this obj.
        QColor color(r, g, b);

        if (color.isValid())
        {
            obj->setObjColor(color.red(), color.green(), color.blue());
            obj->setModified(true);
            setModelModified(modelName, true);
            setObjTreeModified(true);
        }

        return true;
    }

    return false;
}

bool ObjectManager::getObjVis(QString modelName, QString objName)
{
    vector<Model>::iterator modelIter = this->getObjTree()->begin();
    for ( ; modelIter < this->getObjTree()->end(); modelIter++)
    {
        if (0 == modelName.compare(modelIter->getModelName()))
        {
            vector<GeoObject>::iterator objIter = modelIter->getVecOfGeoObjs()->begin();
            for ( ; objIter < modelIter->getVecOfGeoObjs()->end(); objIter++)
            {
                if (0 == objName.compare(objIter->getName()))
                {
                    return objIter->getVisibility();
                }
            }
        }
    }
    return false; //in case of obj not existed.
}

bool ObjectManager::getObjColor( QString modelName, QString objName, int rgb[3] )
{
    GeoObject* obj = findObj(modelName, objName);
    if (obj)
    {
        obj->getObjColor(rgb, rgb+1, rgb+2);
        if (QColor(rgb[0], rgb[1], rgb[2]).isValid())
        {
            findModel(modelName)->setModified(true);
            return true;
        }

    }

    return false;
}

void GeoObject::setObjColor(int r, int g, int b)
{
    this->reader->Update();
    vtkDataArray* scalars = reader->GetOutput()->GetPointData()->GetScalars();
    int size = scalars->GetSize();

    if ((size%3) != 0 || false == QColor(r, g, b).isValid())
    {
        return;
    }

    for (int i = 0; i < (size/3); i++)
    {
        scalars->SetTuple3(i, r,g,b);
    }

    reader->GetOutput()->GetPointData()->SetScalars(scalars);

    this->setModified(true);
}

void GeoObject::getObjColor( int* r, int* g, int* b )
{
    //throw std::exception("The method or operation is not implemented.");
    this->reader->Update();
    vtkDataArray* scalars = reader->GetOutput()->GetPointData()->GetScalars();
    int size = scalars->GetSize();

    if ((size%3) != 0)
    {
        *r = -1;
        *g = -1;
        *b = -1;
        return;
    }

    double* tuple3;
    int sumR, sumG, sumB;
    for (int i = 0; i < (size/3); i++)
    {
         tuple3= scalars->GetTuple3(i);
         sumR = tuple3[0];
         sumG = tuple3[1];
         sumB = tuple3[2];
    }
    *r = sumR / 3;
    *b = sumB / 3;
    *g = sumG / 3;
}

FavGroup* ObjectManager::findFavGroup(QString groupName)
{
    vector<FavGroup>::iterator group_iter = treeOfFav.begin();
    for ( ; group_iter < treeOfFav.end(); group_iter++)
    {
        if (0 == groupName.compare(group_iter->getGroupName().c_str()))
        {
            return &(*group_iter);
        }
    }

    return NULL;
}

bool ObjectManager::removeGroup(QString groupName)
{
    vector<FavGroup>::iterator group_iter = treeOfFav.begin();
    for ( ; group_iter < treeOfFav.end(); group_iter++)
    {
        if (0 == groupName.compare(group_iter->getGroupName().c_str()))
        {
            treeOfFav.erase(group_iter);
            this->setFavTreeModified(true);
            return true;
        }
    }

    return false;
}

bool ObjectManager::addFavGroup(QString groupName)
{
    if (NULL != this->findFavGroup(groupName))
    {
        return false;
    }

    this->treeOfFav.push_back(FavGroup(groupName.toStdString()));
    this->setFavTreeModified(true);
    return true;
}

Model* ObjectManager::findModel(QString modelName)
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter < treeOfGeoObjs.end(); model_iter++)
    {
        if (0 == modelName.compare(model_iter->getModelName()))
        {
            return &(*model_iter);
        }
    }

    return NULL;
}

FavItem* ObjectManager::findFavItem(QString groupName, QString favItemName)
{
    FavGroup* pGroup = findFavGroup(groupName);
    if (pGroup)
    {
        return (pGroup->findFavItem(favItemName));
    }

    return NULL;
}

bool ObjectManager::addFavItem(QString groupName, FavItem& favItem)
{
    FavGroup* pGroup = findFavGroup(groupName);
    if (NULL == pGroup)
    {
        return false;
    }

    if (NULL != pGroup->findFavItem(favItem.getName()))
    {
        return false;
    }

    pGroup->getVecOfItems()->push_back(favItem);
    pGroup->setModified(true);
    this->setFavTreeModified(true);
    return true;
}

bool ObjectManager::removeFavItem(QString groupName, QString favItemName)
{
    FavGroup* group = findFavGroup(groupName);
    if (!group)
    {
        return false;
    }
    vector<FavItem>::iterator item_Iter = group->getVecOfItems()->begin();
    for ( ; item_Iter < group->getVecOfItems()->end(); item_Iter++)
    {
        if (0 == item_Iter->getName().compare(favItemName))
        {
            group->getVecOfItems()->erase(item_Iter);
            group->setModified(true);
            this->setFavTreeModified(true);
            return true;
        }
    }

    return false;
}

bool ObjectManager::updateFavItem(QString groupName,
                                  QString oldFavItemName,
                                  QString newFavItemName,
                                  QString newModelName,
                                  QString newObjName)
{
    FavItem* favItem = findFavItem(groupName, oldFavItemName);
    if (NULL == favItem)
    {
        return false;
    }
    favItem->setName(newFavItemName);
    favItem->setModelName(newModelName);
    favItem->setObjName(newObjName);
    favItem->setModified(true);
    findFavGroup(groupName)->setModified(true);
    setFavTreeModified(true);
    return true;
}

bool ObjectManager::removeObj(const QString modelName, const QString objName)
{
    Model* model = findModel(modelName);
    if (!model)
    {
        return false;
    }
    vector<GeoObject>::iterator obj_iter = model->getVecOfGeoObjs()->begin();
    for ( ; obj_iter < model->getVecOfGeoObjs()->end(); obj_iter++)
    {
        if (0 == obj_iter->getName().compare(objName))
        {
            DeleteVTKPointer(obj_iter->reader);
            model->getVecOfGeoObjs()->erase(obj_iter);
            model->setModified(true);
            this->setObjTreeModified(true);
            return true;
        }
    }

    return false;
}

void ReaderUpdater::run()
{
    m_reader->Update();
    emit UpdateFinished();
}


FavItem* FavGroup::findFavItem(QString itemName)
{
    vector<FavItem>::iterator item_Iter = this->vecOfItems.begin();
    for ( ; item_Iter < vecOfItems.end(); item_Iter++)
    {
        if (0 == item_Iter->getName().compare(itemName))
        {
            return &(*item_Iter);
        }
    }

    return NULL;
}

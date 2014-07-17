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
        model_iter->vecOfGeoObjs.clear();
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
        iter->vecOfGeoObjs.clear();
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
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            QString loadingTip("正在加载模型：                \n");
            loadingTip.append(model_iter->name + "/" + obj_iter->getName());
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
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            DeleteVTKPointer(obj_iter->reader);
        }
        model_iter->vecOfGeoObjs.clear();
    }
    computeObjTreeBound();
}

void ObjectManager::ClearObjTree()
{
    DelAllRdrsInObjTree();
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->vecOfGeoObjs.clear();
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
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
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
        ret += model_iter->vecOfGeoObjs.size();
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

GeoObject* ObjectManager::findObjByName(QString modelName, QString objName)
{
    //time complexity: m + n
    vector<Model>::iterator modelIter = treeOfGeoObjs.begin();
    for ( ; modelIter < treeOfGeoObjs.end(); modelIter++)
    {
        if (0 == modelName.compare(modelIter->name))
        {
            vector<GeoObject>::iterator objIter = modelIter->vecOfGeoObjs.begin();
            for ( ; objIter < modelIter->vecOfGeoObjs.end(); objIter++)
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
        if (0 == modelName.compare(modelIter->name))
        {
            modelIter->hasModified = hasModified;
            return true;
        }
    }

    return false;
}

bool ObjectManager::setObjVisByName(QString modelName, QString objName, bool vis)
{
    GeoObject* obj = findObjByName(modelName, objName);
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

bool ObjectManager::setObjColorByName(QString modelName, QString objName, int r, int g, int b)
{
    GeoObject* obj = findObjByName(modelName, objName);
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

bool ObjectManager::getObjColorByName( QString modelName, QString objName, int rgb[3] )
{
    GeoObject* obj = findObjByName(modelName, objName);
    if (obj)
    {
        obj->getObjColor(rgb, rgb+1, rgb+2);
        if (QColor(rgb[0], rgb[1], rgb[2]).isValid())
        {
            return true;
        }
    }

    return false;
}

void GeoObject::setObjColor(int r, int g, int b)
{
    //throw std::exception("The method or operation is not implemented.");
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

const FavGroup* ObjectManager::findFavGroupByName(QString groupName)
{
    vector<FavGroup>::const_iterator group_iter = treeOfFav.cbegin();
    for ( ; group_iter < treeOfFav.cend(); group_iter++)
    {
        if (0 == groupName.compare(group_iter->getGroupName().c_str()))
        {
            return &(*group_iter);
        }
    }

    return NULL;
}

bool ObjectManager::addFavGroup(QString groupName)
{
    if (NULL != this->findFavGroupByName(groupName))
    {
        return false;
    }

    this->treeOfFav.push_back(FavGroup(groupName.toStdString()));
    return true;
}

void ReaderUpdater::run()
{
    m_reader->Update();
    emit UpdateFinished();
}


#include <QColorDialog>
#include "vtkCellData.h"
#include "vtkObjectsManager.h"
#include "Utils/GVSUtils.h"
#include "Utils/vtkTotallyInclude.h"


ObjectManager::ObjectManager(void)
{
    this->treeModified = false;
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
            loadingTip.append(model_iter->modelName + "/" + obj_iter->getName().c_str());
            pProgressDlg->setLabelText(loadingTip);

            obj_iter->reader->Update();
            pProgressDlg->setValue(pProgressDlg->value() + 1);
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
        if (0 == modelName.compare(modelIter->modelName))
        {
            vector<GeoObject>::iterator objIter = modelIter->vecOfGeoObjs.begin();
            for ( ; objIter < modelIter->vecOfGeoObjs.end(); objIter++)
            {
                if (0 == objName.compare(objIter->getName().c_str()))
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
        if (0 == modelName.compare(modelIter->modelName))
        {
            modelIter->Modified = hasModified;
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
        setTreeModified(true);

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
            setTreeModified(true);
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
    throw std::exception("The method or operation is not implemented.");
}

void GeoObject::getObjColor( int* r, int* g, int* b )
{
    throw std::exception("The method or operation is not implemented.");
}

void ReaderUpdater::run()
{
    m_reader->Update();
    emit UpdateFinished();
}


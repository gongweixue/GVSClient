#include "Utils/vtkTotallyInclude.h"
#include "Utils/GVSUtils.h"
#include "vtkObjectsManager.h"

ObjectManager::ObjectManager(void)
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for (; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();
}

ObjectManager::~ObjectManager(void)
{
    DelAllRdrsInGeoObj();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for (; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();
}


void ObjectManager::UpdateAllReaders()
{
    //update every record's reader.
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        obj_iter->reader->Update();
    }

    computeObjTreeBound();
}


void ObjectManager::DelAllRdrsInGeoObj()
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        DeleteVTKPointer(obj_iter->reader);
    }
    computeObjTreeBound();
}

vector<GeoObject>* ObjectManager::GetObjectsTable()
{
    throw std::exception("GetObjectsTable function should be \
                         replaced by something else since the data \
                         stored in the new tree structure...");
    return NULL;
}

void ObjectManager::ClearObjTree()
{
    DelAllRdrsInGeoObj();
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
        double tmpBnds[6];
        obj_iter->reader->GetOutput()->GetBounds(tmpBnds);

        m_bounds[0] = (tmpBnds[0] < m_bounds[0])? tmpBnds[0] : m_bounds[0];
        m_bounds[1] = (tmpBnds[1] > m_bounds[1])? tmpBnds[1] : m_bounds[1];
        m_bounds[2] = (tmpBnds[2] < m_bounds[2])? tmpBnds[2] : m_bounds[2];
        m_bounds[3] = (tmpBnds[3] > m_bounds[3])? tmpBnds[3] : m_bounds[3];
        m_bounds[4] = (tmpBnds[4] < m_bounds[4])? tmpBnds[4] : m_bounds[4];
        m_bounds[5] = (tmpBnds[5] > m_bounds[5])? tmpBnds[5] : m_bounds[5];

    }
    /*vtkAppendFilter* apdFilter = vtkAppendFilter::New();
    vector<GeoObject>::iterator iter_ObRcd = m_objectsTable.begin();
    for ( ; iter_ObRcd != m_objectsTable.end()&&iter_ObRcd->getVisibility(); iter_ObRcd++)
    {
        apdFilter->AddInput(iter_ObRcd->reader->GetOutput());
    }
    apdFilter->Update();
    apdFilter->GetOutput()->GetBounds(m_bounds);
    apdFilter->Delete();*/
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

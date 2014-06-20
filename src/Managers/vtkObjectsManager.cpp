#include "Utils/vtkTotallyInclude.h"
#include "vtkObjectsManager.h"

ObjectManager::ObjectManager(void)
{
    //m_docsNameVector.clear();
    //m_objectsTable.clear();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for (; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();
}

ObjectManager::~ObjectManager(void)
{
    DeleteAllReaders();
    //m_docsNameVector.clear();
    //m_objectsTable.clear();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for (; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();
}

void ObjectManager::InsertObjectsByNames()
{
    vector<string>::const_iterator iter_docs = m_docsNameVector.begin();
    for (; iter_docs != m_docsNameVector.end(); iter_docs++)
    {
        //InsertGeoObjectByName((*iter_docs));
        //create a Reader.
        vtkDataSetReader* tempReader = vtkDataSetReader::New();
        tempReader->SetFileName((*iter_docs).c_str());
        tempReader->Update();
        //insert the record into the table.
        GeoObject tempObjectRecord((*iter_docs), GEO_OBJECT_TYPE_SURFACE, tempReader);
        m_objectsTable.push_back(tempObjectRecord);
    }
}

void ObjectManager::UpdateAllReaders()
{
    //update every record's reader.
    vector<GeoObject>::iterator iter_ObjectsTable = m_objectsTable.begin();
    for (; iter_ObjectsTable != m_objectsTable.end(); iter_ObjectsTable++)
    {
        iter_ObjectsTable->reader->Update();
    }
    ComputeBounds();
}


void ObjectManager::DeleteAllReaders()
{
    vector<GeoObject>::iterator iter_ObjectsTable = m_objectsTable.begin();
    for (; iter_ObjectsTable != m_objectsTable.end(); iter_ObjectsTable++)
    {
        DeleteVTKPointer(iter_ObjectsTable->reader);
    }
}

vector<GeoObject>* ObjectManager::GetObjectsTable()
{
    return &m_objectsTable;
}

void ObjectManager::ClearObjTree()
{
    DeleteAllReaders();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for ( ; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->vecOfGeoObjs.clear();
    }
}

double* ObjectManager::ComputeBounds()
{
    vtkAppendFilter* apdFilter = vtkAppendFilter::New();
    vector<GeoObject>::iterator iter_ObRcd = m_objectsTable.begin();
    for ( ; iter_ObRcd != m_objectsTable.end()&&iter_ObRcd->getVisibility(); iter_ObRcd++)
    {
        apdFilter->AddInput(iter_ObRcd->reader->GetOutput());
    }
    apdFilter->Update();
    apdFilter->GetOutput()->GetBounds(m_bounds);
    apdFilter->Delete();
    return m_bounds;
}
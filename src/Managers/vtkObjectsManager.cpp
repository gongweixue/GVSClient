#include "Utils/vtkTotallyInclude.h"
#include "vtkObjectsManager.h"

ObjectsManager::ObjectsManager(void)
{
    m_docsNameVector.clear();
    m_objectsTable.clear();
}

ObjectsManager::~ObjectsManager(void)
{
    DeleteAllReaders();
    m_docsNameVector.clear();
    m_objectsTable.clear();
}

void ObjectsManager::ReadObjectNames()
{
    vector<string>::const_iterator iter_docs = m_docsNameVector.begin();
    for (; iter_docs != m_docsNameVector.end(); iter_docs++)
    {
        InsertObjectRecord((*iter_docs));
    }
}

void ObjectsManager::UpdateAllReaders()
{
    //update every record's reader.
    vector<ObjectRecord>::iterator iter_ObjectsTable = m_objectsTable.begin();
    for (; iter_ObjectsTable != m_objectsTable.end(); iter_ObjectsTable++)
    {
        iter_ObjectsTable->reader->Update();
    }
}

void ObjectsManager::InsertObjectRecord(string fileName)
{
    //create a Reader.
    vtkDataSetReader* tempReader = vtkDataSetReader::New();
    tempReader->SetFileName(fileName.c_str());
    tempReader->Update();
    //insert the record into the table.
    ObjectRecord tempObjectRecord(fileName, tempReader);
    m_objectsTable.push_back(tempObjectRecord);
}

void ObjectsManager::DeleteAllReaders()
{
    vector<ObjectRecord>::iterator iter_ObjectsTable = m_objectsTable.begin();
    for (; iter_ObjectsTable != m_objectsTable.end(); iter_ObjectsTable++)
    {
        DeleteVTKPointer(iter_ObjectsTable->reader);
    }
}

vector<ObjectRecord>* ObjectsManager::GetObjectsTable()
{
    return &m_objectsTable;
}

void ObjectsManager::ClearObjectsTable()
{
    DeleteAllReaders();
    m_objectsTable.clear();
}

double* ObjectsManager::ComputeBounds()
{
    vtkAppendFilter* apdFilter = vtkAppendFilter::New();
    vector<ObjectRecord>::iterator iter_ObRcd = m_objectsTable.begin();
    for ( ; iter_ObRcd != m_objectsTable.end()&&iter_ObRcd->visible ; iter_ObRcd++)
    {
        apdFilter->AddInput(iter_ObRcd->reader->GetOutput());
    }
    apdFilter->Update();
    apdFilter->GetOutput()->GetBounds(m_bounds);
    apdFilter->Delete();
    return m_bounds;
}
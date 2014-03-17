#include "Utils/vtkTotallyInclude.h"
#include "vtkSceneManager.h"

SceneManager::SceneManager(void)
{
    m_ActorRecordTable.clear();
    m_CurrentSceneState=SCENE_STATE_NULL;
    m_SceneBounds[0]=0;
    m_SceneBounds[1]=0;
    m_SceneBounds[2]=0;
    m_SceneBounds[3]=0;
    m_SceneBounds[4]=0;
    m_SceneBounds[5]=0;
    m_SceneCenter[0]=0;
    m_SceneCenter[1]=0;
    m_SceneCenter[2]=0;
    m_lut=vtkLookupTable::New();
}

SceneManager::~SceneManager(void)
{
    DeleteVTKPointer(m_lut);
    DeleteAllActors();
    ClearActorTable();
}

void SceneManager::InsertActorRecord(vtkActor* actor,
                                     string dataSetName,
                                     int sceneStateBelong,
                                     bool visible )
{
    ActorRecord tempActorRecord(actor, dataSetName, sceneStateBelong, visible);
    m_ActorRecordTable.push_back(tempActorRecord);
}

void SceneManager::DeleteAllActors()
{
     //delete every actor pointer in every record.
    vector<ActorRecord>::iterator iter_ActorTable = m_ActorRecordTable.begin();
    for ( ; iter_ActorTable != m_ActorRecordTable.end(); iter_ActorTable++)
    {
        DeleteVTKPointer(iter_ActorTable->actor);
    }
    m_SceneBounds[0]=0;
    m_SceneBounds[1]=0;
    m_SceneBounds[2]=0;
    m_SceneBounds[3]=0;
    m_SceneBounds[4]=0;
    m_SceneBounds[5]=0;
    m_SceneCenter[0]=0;
    m_SceneCenter[1]=0;
    m_SceneCenter[2]=0;
}

void SceneManager::ClearActorTable()
{
    DeleteAllActors();
    m_ActorRecordTable.clear();
}

void SceneManager::SetSceneState( int sceneState )
{
    m_CurrentSceneState=sceneState;
}

int SceneManager::GetSceneState()
{
    return m_CurrentSceneState;
}

void SceneManager::SetSceneBounds( double bounds[6] )
{
    for (int i=0; i<6; i++)
    {
        m_SceneBounds[i]=bounds[i];
    }
}

void SceneManager::SetSceneBounds(double xmin,double xmax,
                                  double ymin,double ymax,
                                  double zmin,double zmax )
{
    m_SceneBounds[0]=xmin;
    m_SceneBounds[1]=xmax;
    m_SceneBounds[2]=ymin;
    m_SceneBounds[3]=ymax;
    m_SceneBounds[4]=zmin;
    m_SceneBounds[5]=zmax;
}

double* SceneManager::GetSceneBounds()
{
    return m_SceneBounds;
}

void SceneManager::SetSceneCenter( double center[3] )
{
    m_SceneCenter[0]=center[0];
    m_SceneCenter[1]=center[1];
    m_SceneCenter[2]=center[2];
}

void SceneManager::SetSceneCenter(double cx, double cy, double cz)
{
    m_SceneCenter[0]=cx;
    m_SceneCenter[1]=cy;
    m_SceneCenter[2]=cz;
}

double* SceneManager::GetSceneCenter()
{
    return m_SceneCenter;
}

vtkLookupTable* SceneManager::GetLookupTable()
{
    return m_lut;
}

vector<ActorRecord>* SceneManager::GetActorRecordTable()
{
    return &m_ActorRecordTable;
}

void SceneManager::AddCrrtStatActrToRnder( vtkRenderer* renderer )
{
    vector<ActorRecord>::iterator iter_actorRecord = m_ActorRecordTable.begin();
    for ( ; iter_actorRecord != m_ActorRecordTable.end(); iter_actorRecord++)
    {
        if(iter_actorRecord->sceneStateBelong == m_CurrentSceneState
                && iter_actorRecord->isVisible){
            renderer->AddActor(iter_actorRecord->actor);
        }
    }
}

void SceneManager::AddActorsByState(vtkRenderer* renderer, int state)
{
    vector<ActorRecord>::iterator iter_actorRecord=m_ActorRecordTable.begin();
    for ( ; iter_actorRecord!=m_ActorRecordTable.end(); iter_actorRecord++)
    {
        if(iter_actorRecord->sceneStateBelong==state && iter_actorRecord->isVisible) 
        {
            renderer->AddActor(iter_actorRecord->actor);
        }
    }
}

void SceneManager::RemoveActorsByState(vtkRenderer* renderer, int state)
{
    vector<ActorRecord>::iterator iter_actorRecord = m_ActorRecordTable.begin();
    for ( ; iter_actorRecord != m_ActorRecordTable.end(); iter_actorRecord++)
    {
        if(iter_actorRecord->sceneStateBelong == state && iter_actorRecord->isVisible)
            renderer->RemoveActor(iter_actorRecord->actor);
    }
}

void SceneManager::RemoveAllLabelActorRecord()
{
    vector<ActorRecord>::iterator iter_actorRecord  =m_ActorRecordTable.begin();
    for ( ; iter_actorRecord != m_ActorRecordTable.end(); iter_actorRecord++)
    {
        if (iter_actorRecord->sceneStateBelong == SCENE_STATE_SHOW_LABEL)
        {
            DeleteVTKPointer(iter_actorRecord->actor);
            m_ActorRecordTable.erase(iter_actorRecord);
            iter_actorRecord = m_ActorRecordTable.begin();
        }
    }
}
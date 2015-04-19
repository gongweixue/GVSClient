#ifndef VTK_SCENE_MANAGER_H
#define VTK_SCENE_MANAGER_H

#include <String>
#include <vector>
#include "Utils/vtkTotallyInclude.h"

using namespace std;

// #define  SCENE_STATE_NULL           0
// #define  SCENE_STATE_SHOW_LABEL     1000
// #define  SCENE_STATE_ORIGINAL       2000
// #define  SCENE_STATE_PLANE_CLIP     3000
// #define  SCENE_STATE_EXPLODE        4000
// #define  SCENE_STATE_STD_EXPLODE    4100
// #define  SCENE_STATE_BOX_CLIP       5000
// #define  SCENE_STATE_PRISM_CLIP     6000

enum SceneState {
    SCENE_STATE_NULL,
    SCENE_STATE_SHOW_LABEL,
    SCENE_STATE_ORIGINAL,
    SCENE_STATE_PLANE_CLIP,
    SCENE_STATE_EXPLODE,
    SCENE_STATE_STD_EXPLODE,
    SCENE_STATE_BOX_CLIP,
    SCENE_STATE_PRISM_CLIP
};

typedef struct ActorRecord
{
public:
    string name;
    bool isVisible;
    int sceneStateBelong;
    vtkActor* actor;
public:
    ActorRecord(vtkActor* paraAct, string dsName = "",
                int paraStatebelong = SCENE_STATE_ORIGINAL, bool paraVisible = 1)
        : name(dsName),
          isVisible(paraVisible),
          sceneStateBelong(paraStatebelong),
          actor(paraAct)
    {};
    ~ActorRecord() {
    }
} ActorRecord;

class SceneManager {
public:
    SceneManager(void);
    ~SceneManager(void);
    void InsertActorRcrd(vtkActor* actor, const string& dataSetName = "",
                         int sceneStateBelong = SCENE_STATE_ORIGINAL, bool visible = 1);
    void AddCrrtStatActrToRnder(vtkRenderer* renderer);
    void AddActorsByState(vtkRenderer* renderer, int state);
    void RemoveActorsFromRendererByState(vtkRenderer* renderer, int state);
    void SetActorVisibleByName(const char* name, bool visible);
    bool deleteActorByName(const string& actorName);

    vector<ActorRecord>* GetActorRecordTable();
    void DeleteAllActors();
    void ClearActorTable();

    void SetSceneState(int sceneState);
    int  GetSceneState();

    void SetSceneBounds(double bounds[6]);
    void SetSceneBounds(double xmin, double xmax,
                        double ymin, double ymax,
                        double zmin, double zmax);
    double* GetSceneBounds();
    void  SetSceneCenter(double center[3]);
    void  SetSceneCenter(double cx, double cy, double cz);
    double* GetSceneCenter();

    vtkLookupTable* GetLookupTable();
    ActorRecord* getActorRecordByName(const std::string& actorName);

private:
    vector<ActorRecord> m_ActorRecordTable;
    int m_CurrentSceneState;
    double m_SceneBounds[6];
    double m_SceneCenter[3];
    vtkLookupTable* m_lut;
};

#endif

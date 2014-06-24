#ifndef VTK_OBJECTS_MANAGER_H
#define VTK_OBJECTS_MANAGER_H

#include "Utils/vtkTotallyInclude.h"
#include "String"
#include "QString"
#include <vector>

#define GEO_OBJECT_TYPE_POINT       100
#define GEO_OBJECT_TYPE_LINE        200
#define GEO_OBJECT_TYPE_SURFACE     300

using namespace std;

typedef struct GeoObject
{
public:
    int type;
    vtkDataSetReader* reader;
private:
    string fileName;
    bool visibility;
public:
    GeoObject(string fname, int ty, vtkDataSetReader* rder, bool vis)
    {
        fileName=fname;
        this->type = ty;
        reader=rder;
        visibility=vis;
    };
    ~GeoObject()
    {
    }
    string getName() {return fileName;};
    bool getVisibility() {return visibility;};
    void setVisibility(bool vis) {visibility = vis;};
} GeoObject;

typedef struct Model
{
public:
    QString modelName;
    vector<GeoObject> vecOfGeoObjs;
} Model;

class ObjectManager
{
public:
    friend class GVSDoc;
    friend class MainWindow;
    ObjectManager(void);
    ~ObjectManager(void);
    void ClearObjTree();
    void UpdateAllReaders();
    void DelAllRdrsInObjTree();

private:
    double* computeObjTreeBound();
    int getNumOfObjsInTree();
private:
    double m_bounds[6];
    vector<Model> treeOfGeoObjs;
};

#endif //VTK_OBJECTS_MANAGER_H

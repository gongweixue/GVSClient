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
    GeoObject(string fname, int type, vtkDataSetReader* rder, bool vis=1)
    {
        fileName=fname;
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
    void InsertObjectsByNames();
    void UpdateAllReaders();
    void DeleteAllReaders();
    vector<GeoObject>* GetObjectsTable();
private:
    double* ComputeBounds();
public:
    vector<Model> treeOfGeoObjs;
private:
    double m_bounds[6];
    vector<string> m_docsNameVector;
    vector<GeoObject> m_objectsTable;
};

#endif //VTK_OBJECTS_MANAGER_H

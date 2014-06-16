#pragma once
#include "Utils/vtkTotallyInclude.h"
#include "String"
#include <vector>

#define GEO_OBJECT_TYPE_POINT    100
#define GEO_OBJECT_TYPE_LINE    200
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
} GeoObject ;

class ObjectsManager
{
public:
    ObjectsManager(void);
    ~ObjectsManager(void);
    void InsertGeoObject(string fileName);
    void ClearObjectsTable();
    void ReadObjectNames();
    void UpdateAllReaders();
    void DeleteAllReaders();
    vector<GeoObject>* GetObjectsTable();
private:
    double* ComputeBounds();

public:
    vector<string> m_docsNameVector;
    double m_bounds[6];
private:
    vector<GeoObject> m_objectsTable;
};

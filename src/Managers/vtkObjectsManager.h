#ifndef VTK_OBJECTS_MANAGER_H
#define VTK_OBJECTS_MANAGER_H

#include "String"
#include <vector>
#include <QObject>
#include <QProgressDialog>
#include <QString>
#include "Utils/vtkTotallyInclude.h"

//#define GEO_OBJECT_TYPE_POINT       100
//#define GEO_OBJECT_TYPE_LINE        200
//#define GEO_OBJECT_TYPE_SURFACE     300

typedef enum GVS_OBJ_TYPE {
    GEO_OBJECT_TYPE_POINT,
    GEO_OBJECT_TYPE_LINE,
    GEO_OBJECT_TYPE_SURFACE
} GVS_OBJ_TYPE;

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


class ObjectManager : public QObject
{
    Q_OBJECT
public:
    friend class GVSDoc;
    friend class MainWindow;
    ObjectManager(void);
    ~ObjectManager(void);
    void ClearObjTree();
    void LoadDataForReadersInTree();
    void DelAllRdrsInObjTree();
    int getNumOfObjsInTree();
    vector<Model>* getObjTree() {return &treeOfGeoObjs;};

public slots:
    void OnObjUpdateFinished();

private:
    double* computeObjTreeBound();
private:
    double m_bounds[6];
    vector<Model> treeOfGeoObjs;
    QProgressDialog* pProgressDlg;
};


#include <QThread>
#include <QMutex>

class ReaderUpdater : public QThread
{
    Q_OBJECT
public:
    ReaderUpdater(ObjectManager* sender, vtkDataSetReader* reader)
        :m_reader(reader), m_sender(sender)
    {connect(this, SIGNAL(UpdateFinished()), m_sender, SLOT(OnObjUpdateFinished()));};

signals:
    void UpdateFinished();

protected:
    virtual void run();

private:
    vtkDataSetReader* m_reader;
    ObjectManager* m_sender;
};

#endif //VTK_OBJECTS_MANAGER_H

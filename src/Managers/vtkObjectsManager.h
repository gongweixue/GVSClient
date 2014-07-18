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
    vtkDataSetReader* reader;
private:
    QString fileName;
    bool visibility;
    bool Modified;
    int type;
public:
    GeoObject(QString fname, int ty, vtkDataSetReader* rder, bool vis)
    {
        fileName = fname;
        this->type = ty;
        reader=rder;
        visibility=vis;
        Modified = false;
    };
    ~GeoObject()
    {
    }
    QString getName() const {return fileName;}
    int getType() {return this->type;}
    bool getVisibility() {return visibility;}
    void setVisibility(bool vis) {visibility = vis;}
    bool getModified() {return this->Modified;}
    void setModified(bool isModified) {this->Modified = isModified;}
    void setObjColor(int r, int g, int b);
    void getObjColor(int* r, int* g, int* b);
} GeoObject;

typedef struct Model
{
public:
    Model(QString modelName)
    {
        name = modelName;
        hasModified = false;
    }
    QString name;
    vector<GeoObject> vecOfGeoObjs;
    bool hasModified;
} Model;

typedef struct FavItem
{
public:
    FavItem(QString itemName, QString mdlNameVal, QString objNameVal)
    {
        name = itemName;
        modelName = mdlNameVal;
        objName = objNameVal;
        hasModified = false;
    }

    void setModified(bool modifiedVal) {this->hasModified = modifiedVal;}
    bool getModified() {return hasModified;}

    void setName(QString newName) {name = newName;}
    QString getName() {return name;}

    QString getObjPath() {return modelName + "/" + objName;}
    void setObjPath(QString newModelName, QString newObjName)
    {
        modelName = newModelName;
        objName = newObjName;
    }

    void setModelName(QString newModelName) {modelName = newModelName;}
    QString getModelName() const {return modelName;}

    void setObjName(QString newObjName) {objName = newObjName;}
    QString getObjName() const {return objName;}

private:
    QString name;
    QString modelName;
    QString objName;
    bool hasModified;
} FavItem;

typedef struct FavGroup
{
public:
    FavGroup(std::string name)
    {
        this->groupName = name;
        this->hasModified = false;
    }

    void setGroupName(std::string name) {this->groupName = name;}
    std::string getGroupName() const {return groupName;}

    void setModified(bool modifiedVal) {this->hasModified = modifiedVal;}
    bool getModified() {return hasModified;}

    FavItem* findFavItem(QString itemName);

public:
    vector<FavItem> vecOfItems;

private:
    std::string groupName;

    bool hasModified;
} FavGroup;


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

    vector<Model>* getObjTree() {return &treeOfGeoObjs;}
    vector<FavGroup>* getFavTree() {return &treeOfFav;}

    GeoObject* findObjByName(QString modelName, QString objName);
    Model* findModelByName(QString modelName);
    FavGroup* findFavGroupByName(QString groupName);
    FavItem* findFavItemByName(QString groupName, QString favItemName);

    bool setObjVisByName(QString modelName, QString objName, bool vis);

    bool setModelModified(QString modelName, bool hasModified);

    void setObjTreeModified(bool isModified) {this->objTreeModified = isModified;}
    bool getObjTreeModified() {return this->objTreeModified;}

    void setFavTreeModified(bool isModified) {this->favTreeModified = isModified;}
    bool getFavTreeModified() {return this->favTreeModified;}

    bool setObjColorByName(QString modelName, QString objName, int r, int g, int b);
    bool getObjColorByName(QString modelName, QString objName, int rgb[3]);

    bool addFavGroup(QString groupName);
    bool addFavItem(QString groupName, FavItem& favItem);

public slots:
    void OnObjUpdateFinished();

private:
    double* computeObjTreeBound();

private:
    double m_bounds[6];

    vector<Model> treeOfGeoObjs;
    bool objTreeModified;

    vector<FavGroup> treeOfFav;
    bool favTreeModified;

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

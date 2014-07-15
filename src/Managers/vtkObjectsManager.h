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
    bool Modified;
public:
    GeoObject(string fname, int ty, vtkDataSetReader* rder, bool vis)
    {
        fileName=fname;
        this->type = ty;
        reader=rder;
        visibility=vis;
        Modified = false;
    };
    ~GeoObject()
    {
    }
    string getName() {return fileName;};
    bool getVisibility() {return visibility;};
    void setVisibility(bool vis) {visibility = vis;};
    bool getModified() {return this->Modified;};
    void setModified(bool isModified) {this->Modified = isModified;};
} GeoObject;

typedef struct Model
{
public:
    Model(const char* modelName)
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
    FavItem(const char* itemName, const char* mdlNameVal, const char objNameVal)
    {
        name = itemName;
        modelName = mdlNameVal;
        objName = objNameVal;
        hasModified = false;
    }

    void setModified(bool modifiedVal) {this->hasModified = modifiedVal;}
    bool getModified() {return hasModified;}

    void setName(const char* newName) {name = newName;}
    QString getName() {return name;}

    void setObjPath(const char* newModelName, const char* newObjName)
    {
        modelName = newModelName;
        objName = newObjName;
    }
    QString getObjPath() {return modelName + "/" + objName;}

private:
    QString name;
    QString modelName;
    QString objName;
    bool hasModified;
} FavItem;

typedef struct FavFolder
{
public:
    FavFolder(const char* name)
    {
        this->folderName = name;
        this->hasModified = false;
    }

    void setFolderName(const char* name) {this->folderName = name;}
    QString getFolderName() {return folderName;}

    void setModified(bool modifiedVal) {this->hasModified = modifiedVal;}
    bool getModified() {return hasModified;}

private:
    QString folderName;
    vector<FavItem> vecOfItems;
    bool hasModified;
} FavFolder;


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
    void setObjTreeModified(bool isModified) {this->objTreeModified = isModified;}
    bool getObjTreeModified() {return this->objTreeModified;}
    void setFavTreeModified(bool isModified) {this->favTreeModified = isModified;}
    bool getFavTreeModified() {return this->favTreeModified;}

public slots:
    void OnObjUpdateFinished();

private:
    double* computeObjTreeBound();
private:
    double m_bounds[6];

    vector<Model> treeOfGeoObjs;
    bool objTreeModified;

    vector<FavFolder> treeOfFav;
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

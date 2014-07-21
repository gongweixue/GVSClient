#ifndef VTK_OBJECTS_MANAGER_H
#define VTK_OBJECTS_MANAGER_H

#include "String"
#include <vector>
#include <QObject>
#include <QProgressDialog>
#include <QString>
#include "Utils/vtkTotallyInclude.h"

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
    const QString fileName;
    bool visibility;
    bool Modified;
    const int type;
public:
    GeoObject(QString fname, int ty, vtkDataSetReader* rder, bool vis)
        : fileName(fname), type(ty), reader(rder), visibility(vis), Modified(false)
    {
    }

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

public:
    GeoObject operator=(GeoObject& geoObj)
    {
        return GeoObject(geoObj.getName(),
                         geoObj.getType(),
                         geoObj.reader,
                         geoObj.getVisibility());
    }

} GeoObject;

typedef struct Model
{
public:
    Model(QString modelName)
        : name(modelName), hasModified(false)
    {
    }

    QString getModelName() {return name;}
    vector<GeoObject>* getVecOfGeoObjs() {return &vecOfGeoObjs;}
    void setModified(bool isModified) {this->hasModified = isModified;}
    bool getModified() {return hasModified;}

public:
    Model operator=(Model& model)
    {
        throw std::exception("This function could not be invoked.");
        return Model(model.getModelName());
    }
private:
    const QString name;
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

    vector<FavItem>* getVecOfItems() {return &vecOfItems;}

private:
    std::string groupName;
    vector<FavItem> vecOfItems;
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
    bool getObjVisByName(QString modelName, QString objName);

    bool setModelModified(QString modelName, bool hasModified);

    void setObjTreeModified(bool isModified) {this->objTreeModified = isModified;}
    bool getObjTreeModified() {return this->objTreeModified;}

    void setFavTreeModified(bool isModified) {this->favTreeModified = isModified;}
    bool getFavTreeModified() {return this->favTreeModified;}

    bool setObjColorByName(QString modelName, QString objName, int r, int g, int b);
    bool getObjColorByName(QString modelName, QString objName, int rgb[3]);

    bool addFavGroup(QString groupName);
    bool removeGroup(QString groupName);
    bool addFavItem(QString groupName, FavItem& favItem);
    bool removeFavItem(QString groupName, QString favItemName);
    bool updateFavItem(QString groupName,
                       QString oldFavItemName,
                       QString newFavItemName,
                       QString newModelName,
                       QString newObjName);

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

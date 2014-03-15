#pragma once
#include "Utils/vtkTotallyInclude.h"
#include "String"
#include <vector>

#define OBJECT_TYPE_POINTSET 100
#define OBJECT_TYPE_POLYLINE 200
#define OBJECT_TYPE_SURFACE 300
#define OBJECT_TYPE_VOLUME 400

using namespace std;

typedef struct ObjectRecord
{
public:
	string fileName;
	vtkDataSetReader* reader;
	bool visible;
public:
	ObjectRecord(string fname, vtkDataSetReader* rder, bool vsble=1)
	{
		fileName=fname;
		reader=rder;
		visible=vsble;
	};
	~ObjectRecord()
	{
	}
} ObjectRecord ;

class ObjectsManager
{
public:
	ObjectsManager(void);
	~ObjectsManager(void);

	void InsertObjectRecord(string fileName);
	int GetObjectType(string fileName);
	vector<ObjectRecord>::iterator FindObjectRecord(string fileName);
	void RemoveObjectRecord(string fileName);
	void ClearObjectsTable();
	void ReadAllDocsIntoReaders();
	void UpdateAllReaders();
	void DeleteAllReaders();
	vector<ObjectRecord>* GetObjectsTable();
	double* ComputeBounds();

	vector<string> m_docsNameVector;
	double m_bounds[6];
private:
	vector<ObjectRecord> m_objectsTable;
};

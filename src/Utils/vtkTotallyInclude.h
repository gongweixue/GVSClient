#pragma once

#include "vtkActor.h"
#include "vtkAppendFilter.h"
#include "vtkAxesActor.h"
#include "vtkBoxWidget.h"
#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkCubeAxesActor.h"
#include "vtkDataSet.h"
#include "vtkDataSetMapper.h"
#include "vtkDataSetReader.h"
#include "vtkDataSetWriter.h"
#include "vtkImplicitPlaneRepresentation.h"
#include "vtkImplicitPlaneWidget2.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkLookupTable.h"
#include "vtkMapper.h"
#include "vtkOpenGLLight.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkPlane.h"
#include "vtkPlanes.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTableBasedClipDataSet.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkWindowToImageFilter.h"

const int evenRowLMT=5;
const int evenColLMT=5;

template<typename T>
bool DeleteVTKPointer(T& pointer)
{
	if (pointer!=0)
	{
		pointer->Delete();
		pointer=0;
		return true;
	}
	return false;
};





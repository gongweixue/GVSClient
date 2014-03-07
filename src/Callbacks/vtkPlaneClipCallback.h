#pragma once
#include "Utils/vtkTotallyInclude.h"

class vtkPlaneClipCallback : public vtkCommand
{
public:
	static vtkPlaneClipCallback *New() 
	{ return new vtkPlaneClipCallback; }
	void Delete() {}
    //update the plane used to clip
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkImplicitPlaneWidget2 *planeWidget = 
			reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
		vtkImplicitPlaneRepresentation *rep = 
			reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
		rep->GetPlane(this->Plane);
		rep->DrawPlaneOff();
	}
	vtkPlaneClipCallback():Plane(0),Actor(0) {}
	vtkPlane *Plane;
	vtkActor *Actor;

};
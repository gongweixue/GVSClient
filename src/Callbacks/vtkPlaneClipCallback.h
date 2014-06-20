#ifndef VTK_PLANE_CLIP_CALLBACK_H
#define VTK_PLANE_CLIP_CALLBACK_H
#include "Utils/vtkTotallyInclude.h"

class vtkPlaneClipCallback : public vtkCommand
{
public:
    static vtkPlaneClipCallback* New() {return new vtkPlaneClipCallback;}
    void Delete() {}
    //update the plane used to clip
    virtual void Execute(vtkObject* caller, unsigned long, void*)
    {
        vtkImplicitPlaneWidget2* planeWidget =
                reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
        vtkWidgetRepresentation* baseRep = planeWidget->GetRepresentation();
        vtkImplicitPlaneRepresentation *rep =
                reinterpret_cast<vtkImplicitPlaneRepresentation*>(baseRep);
        rep->GetPlane(this->Plane);
        rep->DrawPlaneOff();
    }
    vtkPlaneClipCallback():Plane(0),Actor(0) {}
    vtkPlane* Plane;
    vtkActor* Actor;
};
#endif

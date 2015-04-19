#include "vtkBoxClipCallback.h"

vtkBoxClipCallback* vtkBoxClipCallback::New() {
    return new vtkBoxClipCallback;
}
//get the box widget's 6 planes and update the pipeline.
void vtkBoxClipCallback::Execute(vtkObject* caller, unsigned long, void*) {
    DeleteVTKPointer(planes);
    planes = vtkPlanes::New();
    vtkBoxWidget* boxWidget = reinterpret_cast<vtkBoxWidget*>(caller);
    for (int i = 0; i < 6; i++) {
        boxWidget->GetPlanes(planes);
        planes->GetPlane(i, boxPlaneArray[i]);
    }
}

vtkBoxClipCallback::vtkBoxClipCallback() {
    //the planes must be "Newed" before being used by box widget.
    planes = vtkPlanes::New();
    boxPlaneArray = 0;
    boxWidget = 0;
}

void vtkBoxClipCallback::Delete() {
    DeleteVTKPointer(planes);
}

vtkBoxClipCallback::~vtkBoxClipCallback()
{
}

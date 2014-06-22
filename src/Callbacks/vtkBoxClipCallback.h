#ifndef VTK_BOX_CLIP_CALLBACK_H
#define VTK_BOX_CLIP_CALLBACK_H

#include "Utils/vtkTotallyInclude.h"
#include "Utils/GVSUtils.h"
#include "vtkCommand.h"
#include "vtkPlane.h"

class vtkBoxClipCallback : public vtkCommand
{
public:
    static vtkBoxClipCallback* New();
    void Delete();
    virtual void Execute(vtkObject* caller, unsigned long, void*);

    vtkBoxClipCallback();
    ~vtkBoxClipCallback();
    vtkPlane**  boxPlaneArray;
    vtkBoxWidget* boxWidget;
    vtkPlanes* planes;
};

#endif

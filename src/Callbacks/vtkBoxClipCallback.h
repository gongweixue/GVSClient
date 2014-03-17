#pragma once
#include "Utils/vtkTotallyInclude.h"
#include "vtkCommand.h"
#include "vtkPlane.h"

class vtkBoxClipCallback : public vtkCommand
{
public:
    static vtkBoxClipCallback *New();
    void Delete();
    virtual void Execute(vtkObject *caller, unsigned long, void*);

    vtkBoxClipCallback();
    ~vtkBoxClipCallback();
    vtkPlane**  boxPlaneArray;
    vtkBoxWidget* boxWidget;
    vtkPlanes* planes;
};
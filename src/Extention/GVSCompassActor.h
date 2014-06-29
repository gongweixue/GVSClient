#ifndef GVS_COMPASS_ACTOR_H
#define GVS_COMPASS_ACTOR_H

#include "vtkProp3D.h"

class vtkActor;
class vtkCaptionActor2D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;
class vtkSphereSource;

class GVSCompassActor : public vtkProp3D
{
public:
    static GVSCompassActor *New();
    // Support the standard render methods.
    virtual int RenderOpaqueGeometry(vtkViewport *viewport);
    virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
    virtual int RenderOverlay(vtkViewport *viewport);
    virtual int HasTranslucentPolygonalGeometry();
    void ReleaseGraphicsResources(vtkWindow *);
    void GetBounds(double bounds[6]);
    double *GetBounds();
protected:
  GVSCompassActor();
  ~GVSCompassActor();
  void UpdateProps();

  //GVSCompass member
  vtkCaptionActor2D* northLabel;
  vtkCaptionActor2D* southLabel;
  vtkCaptionActor2D* westLabel;
  vtkCaptionActor2D* eastLabel;
  vtkActor* compassActor;
private:
  GVSCompassActor(const GVSCompassActor&);
  void operator=(const GVSCompassActor&);
};

#endif

#include "GVSCompassActor.h"
#include "vtkActor.h"
#include <vtkAppendPolyData.h>
#include "vtkCaptionActor2D.h"
#include <vtkCellData.h>
#include "vtkConeSource.h"
#include "vtkCylinderSource.h"
#include "vtkFloatArray.h"
#include "vtkLineSource.h"
#include "vtkLookupTable.h"
#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPropCollection.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextProperty.h"
#include "vtkTransform.h"

//GVSCompassActor::New()
vtkStandardNewMacro(GVSCompassActor);

GVSCompassActor::GVSCompassActor() {
    //lookup table for setting the north side and others different color.
    vtkSmartPointer<vtkLookupTable> lut = 
        vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(3);
    lut->Build();
    lut->SetTableValue(0, 0.2, 0.83, 1, 1);//7 side
    lut->SetTableValue(1, 1, 0.32, 0, 1);//north side
    lut->SetTableValue(2, 1, 1, 1, 1);//null

    vtkSmartPointer<vtkConeSource> NN = 
        vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkFloatArray> NNscalars = 
        vtkSmartPointer<vtkFloatArray>::New();
    for (int i = 0; i < 5; i++)
    {
        //just NORTH is different(rgb is {0.2, 0.83, 1} in lut).
        NNscalars->InsertNextValue(0);
    }
    NN->SetHeight(10);
    NN->SetRadius(1);
    NN->SetCenter(0, 5, 0);
    NN->SetDirection(0, 1, 0);
    NN->SetResolution(4);
    NN->Update();//should be update for getting the new data.
    NN->GetOutput()->GetCellData()->SetScalars(NNscalars);

    //other 7 axes
    vtkSmartPointer<vtkConeSource> SS = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> EE = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> WW = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> NE = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> NW = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> SE = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkConeSource> SW = vtkSmartPointer<vtkConeSource>::New();
    //scalars of other 7 axes
    vtkSmartPointer<vtkFloatArray> otherScalars = 
        vtkSmartPointer<vtkFloatArray>::New();
    for (int i = 0; i < 5; i++)
    {
        otherScalars->InsertNextValue(1);
    }

    SS->SetHeight(6);
    SS->SetRadius(1);
    SS->SetCenter(0, -3, 0);
    SS->SetDirection(0, -1, 0);
    SS->SetResolution(4);
    SS->Update();
    SS->GetOutput()->GetCellData()->SetScalars(otherScalars);

    EE->SetHeight(6);
    EE->SetRadius(1);
    EE->SetCenter(3, 0, 0);
    EE->SetDirection(1, 0, 0);
    EE->SetResolution(4);
    EE->Update();
    EE->GetOutput()->GetCellData()->SetScalars(otherScalars);

    WW->SetHeight(6);
    WW->SetRadius(1);
    WW->SetCenter(-3, 0, 0);
    WW->SetDirection(-1, 0, 0);
    WW->SetResolution(4);
    WW->Update();
    WW->GetOutput()->GetCellData()->SetScalars(otherScalars);

    NE->SetHeight(4);
    NE->SetRadius(1);
    NE->SetCenter(1.5, 1.5, 0);
    NE->SetDirection(1, 1, 0);
    NE->SetResolution(4);
    NE->Update();
    NE->GetOutput()->GetCellData()->SetScalars(otherScalars);

    NW->SetHeight(4);
    NW->SetRadius(1);
    NW->SetCenter(-1.5, 1.5, 0);
    NW->SetDirection(-1, 1, 0);
    NW->SetResolution(4);
    NW->Update();
    NW->GetOutput()->GetCellData()->SetScalars(otherScalars);

    SW->SetHeight(4);
    SW->SetRadius(1);
    SW->SetCenter(-1.5, -1.5, 0);
    SW->SetDirection(-1, -1, 0);
    SW->SetResolution(4);
    SW->Update();
    SW->GetOutput()->GetCellData()->SetScalars(otherScalars);

    SE->SetHeight(4);
    SE->SetRadius(1);
    SE->SetCenter(1.5, -1.5, 0);
    SE->SetDirection(1, -1, 0);
    SE->SetResolution(4);
    SE->Update();
    SE->GetOutput()->GetCellData()->SetScalars(otherScalars);

    //get all 8 sides together
    vtkSmartPointer<vtkAppendPolyData> append = 
        vtkSmartPointer<vtkAppendPolyData>::New();
    append->AddInput(SS->GetOutput());
    append->AddInput(EE->GetOutput());
    append->AddInput(WW->GetOutput());
    append->AddInput(NE->GetOutput());
    append->AddInput(NW->GetOutput());
    append->AddInput(SW->GetOutput());
    append->AddInput(SE->GetOutput());
    append->AddInput(NN->GetOutput());

    //set lut for mapper.
    vtkSmartPointer<vtkPolyDataMapper> mapper = 
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(append->GetOutput());
    mapper->SetScalarRange(0, 2);
    mapper->SetLookupTable(lut);

    this->compassActor = vtkActor::New();
    this->compassActor->SetMapper(mapper);
    this->compassActor->GetProperty()->SetAmbient(0.2);
    this->compassActor->GetProperty()->SetDiffuse(1);


    //We should define the 2D text following the compass in the marker.
    this->northLabel = vtkCaptionActor2D::New();
    this->northLabel->ThreeDimensionalLeaderOff();
    this->northLabel->LeaderOff();
    this->northLabel->BorderOff();
    this->northLabel->SetPosition(-5, 3);
    this->northLabel->GetCaptionTextProperty()->SetBold(false);

    this->southLabel = vtkCaptionActor2D::New();
    this->southLabel->ThreeDimensionalLeaderOff();
    this->southLabel->LeaderOff();
    this->southLabel->BorderOff();
    this->southLabel->SetPosition(-5, -10);
    this->southLabel->GetCaptionTextProperty()->SetBold(false);


    this->westLabel = vtkCaptionActor2D::New();
    this->westLabel->ThreeDimensionalLeaderOff();
    this->westLabel->LeaderOff();
    this->westLabel->BorderOff();
    this->westLabel->SetPosition(-10, 0);
    this->westLabel->GetCaptionTextProperty()->SetBold(false);

    this->eastLabel = vtkCaptionActor2D::New();
    this->eastLabel->ThreeDimensionalLeaderOff();
    this->eastLabel->LeaderOff();
    this->eastLabel->BorderOff();
    this->eastLabel->SetPosition(0, 0);
    this->eastLabel->GetCaptionTextProperty()->SetBold(false);

    this->UpdateProps();
}

GVSCompassActor::~GVSCompassActor() {
    this->northLabel->Delete();
    this->southLabel->Delete();
    this->westLabel->Delete();
    this->eastLabel->Delete();

    this->compassActor->Delete();
}

int GVSCompassActor::RenderOpaqueGeometry(vtkViewport* vp) {
    int renderedSomething = 0;
    this->UpdateProps();

    renderedSomething += this->compassActor->RenderOpaqueGeometry(vp);

    renderedSomething += this->northLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->southLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->westLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->eastLabel->RenderOpaqueGeometry(vp);

    return ((renderedSomething > 0) ? (1) : (0));
}

int GVSCompassActor::RenderTranslucentPolygonalGeometry(vtkViewport* vp) {
    int renderedSomething = 0;
    this->UpdateProps();

    renderedSomething += this->compassActor->RenderOpaqueGeometry(vp);

    renderedSomething += this->northLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->southLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->westLabel->RenderOpaqueGeometry(vp);
    renderedSomething += this->eastLabel->RenderOpaqueGeometry(vp);

    return ((renderedSomething > 0) ? (1) : (0));
}

int GVSCompassActor::HasTranslucentPolygonalGeometry() {
    int result = 0;
    this->UpdateProps();

    result |= this->compassActor->HasTranslucentPolygonalGeometry();

    result |= this->northLabel->HasTranslucentPolygonalGeometry();
    result |= this->southLabel->HasTranslucentPolygonalGeometry();
    result |= this->westLabel->HasTranslucentPolygonalGeometry();
    result |= this->eastLabel->HasTranslucentPolygonalGeometry();

    return result;
}

int GVSCompassActor::RenderOverlay(vtkViewport* vp) {
    int renderedSomething = 0;
    this->UpdateProps();

    renderedSomething += this->northLabel->RenderOverlay(vp);
    renderedSomething += this->southLabel->RenderOverlay(vp);
    renderedSomething += this->westLabel->RenderOverlay(vp);
    renderedSomething += this->eastLabel->RenderOverlay(vp);

    return ((renderedSomething > 0) ? (1) : (0));
}

void GVSCompassActor::ReleaseGraphicsResources(vtkWindow* win) {
    this->compassActor->ReleaseGraphicsResources(win);

    this->northLabel->ReleaseGraphicsResources(win);
    this->southLabel->ReleaseGraphicsResources(win);
    this->westLabel->ReleaseGraphicsResources(win);
    this->eastLabel->ReleaseGraphicsResources(win);
}

void GVSCompassActor::GetBounds(double bounds[6]) {
    double* bds = this->GetBounds();
    bounds[0] = bds[0];
    bounds[1] = bds[1];
    bounds[2] = bds[2];
    bounds[3] = bds[3];
    bounds[4] = bds[4];
    bounds[5] = bds[5];
}

double* GVSCompassActor::GetBounds(){
    return compassActor->GetBounds();
}

void GVSCompassActor::UpdateProps() {
    this->northLabel->SetCaption("N");
    this->northLabel->SetAttachmentPoint(0, 10, 0);

    this->southLabel->SetCaption("S");
    this->southLabel->SetAttachmentPoint(0, -6, 0);

    this->westLabel->SetCaption("W");
    this->westLabel->SetAttachmentPoint(-6, 0, 0);

    this->eastLabel->SetCaption("E");
    this->eastLabel->SetAttachmentPoint(6, 0, 0);

    vtkLinearTransform* transform = this->GetUserTransform();
    if (transform) {
        this->compassActor->SetUserTransform(transform);
        double newPos[3];

        transform->TransformPoint(this->northLabel->GetAttachmentPoint(), newPos);
        this->northLabel->SetAttachmentPoint(newPos);

        transform->TransformPoint(this->southLabel->GetAttachmentPoint(), newPos);
        this->southLabel->SetAttachmentPoint(newPos);

        transform->TransformPoint(this->westLabel->GetAttachmentPoint(), newPos);
        this->westLabel->SetAttachmentPoint(newPos);

        transform->TransformPoint(this->eastLabel->GetAttachmentPoint(), newPos);
        this->eastLabel->SetAttachmentPoint(newPos);
    }
}


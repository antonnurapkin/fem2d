#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include "../preprocessor/preprocessor.h"

class BoundaryConditionsManager {
public:
    void addForces(vtkSmartPointer<vtkRenderer>& renderer, Preprocessor& preprocessor);
    vtkSmartPointer<vtkActor> createForceActor(double startPoint[3], double angle);
    double calculateAngle(Force force, char type);
    
    void addSupports(vtkSmartPointer<vtkRenderer>& renderer, Preprocessor& preprocessor);
    vtkSmartPointer<vtkActor> createSupportActor(double startPoint[3], bool isXAxis);
};
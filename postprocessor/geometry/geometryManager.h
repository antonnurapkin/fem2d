#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

class Solver;
class Preprocessor;

class GeometryManager {
public:
    void createGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor);
    void createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor, Solver& solver);
    double calculateScaleFactor(double max_length, Solver& solver);
};
#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include "solver/solver.h"
#include "preprocessor/preprocessor.h"

namespace geometry {
    void createGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor);

    double createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor, Solver& solver);

    double calculateScaleFactor(double max_length, Solver& solver);
}; // namespace geometry
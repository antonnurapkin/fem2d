#pragma once

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include "preprocessor/preprocessor.h"
#include "solver/solver.h"

namespace geometry {
void createGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor);

double createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor, Solver& solver);

double calculateScaleFactor(double max_length, Solver& solver);
};  // namespace geometry
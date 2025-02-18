#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>

#include "vizualizationParams.h"
#include "../geometry/geometryManager.h"
#include "../boundaries/boundaryConditionsManager.h"


class Preprocessor;
                                                                       
class RendererManager {
public:
    GeometryManager geometryManager;
    BoundaryConditionsManager boundaryConditionsManager;

    vtkSmartPointer<vtkRenderer> createDataRenderer(vtkSmartPointer<vtkPolyData> polydata, std::vector<double> data, const double viewport[4], const char * name, Preprocessor& preprocessor);
    vtkSmartPointer<vtkRenderer> createDeformedShapeRenderer(double scale, vtkSmartPointer<vtkPolyData> polydataOriginal, vtkSmartPointer<vtkPolyData> polydataDeformed, const double viewport[4], Preprocessor& preprocessor);

    void addAxes(vtkSmartPointer<vtkRenderer>& renderer);
    
    vtkSmartPointer<vtkRenderer> createRenderer(const double viewport[4]);
    vtkSmartPointer<vtkActor> createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper);
    
    vtkSmartPointer<vtkColorTransferFunction> createColorTransferFunction(double min_value, double max_value);
    void createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char* name);
};
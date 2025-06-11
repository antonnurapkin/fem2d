#pragma once

#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include "preprocessor/preprocessor.h"
#include "vizualization_params.h"

class RendererManager {
   public:
    vtkSmartPointer<vtkRenderer> createDataRenderer(vtkSmartPointer<vtkPolyData> polydata, std::vector<double> data, const double viewport[4],
                                                    const char* name, Preprocessor& preprocessor);
    vtkSmartPointer<vtkRenderer> createDeformedShapeRenderer(double scale, vtkSmartPointer<vtkPolyData> polydataOriginal,
                                                             vtkSmartPointer<vtkPolyData> polydataDeformed, const double viewport[4],
                                                             Preprocessor& preprocessor);

    vtkSmartPointer<vtkRenderer> createRenderer(const double viewport[4]);
    vtkSmartPointer<vtkActor> createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper);

    vtkSmartPointer<vtkColorTransferFunction> createColorTransferFunction(double min_value, double max_value);

    void addAxes(vtkSmartPointer<vtkRenderer>& renderer);
    void createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char* name);
};
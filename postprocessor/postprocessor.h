#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkScalarsToColors.h>
#include "../solver/Solver.h"


class Postprocessor{
private:
    Preprocessor preprocessor;

    const int WINDOW_SIZE = 600;

public:
    Postprocessor(Preprocessor& preprocessor);

    void run();

    void createGeoemetry(vtkSmartPointer<vtkPolyData> polydata);

    void createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char * name);

    vtkSmartPointer<vtkRenderer> createDataRenderer(std::vector<double> data, double viewport[4], const char * name);

    vtkSmartPointer<vtkColorTransferFunction> createColorTransferFunction(double min_value, double max_value);

    void addAxes(vtkSmartPointer<vtkRenderer>& renderer);

    vtkSmartPointer<vtkRenderer> createRenderer(double viewport[4]);

    vtkSmartPointer<vtkActor> createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper);
};
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
    Solver solver;

    const int WINDOW_SIZE = 600;
    const double LINE_WIDTH = 3.0;

    double STRESS_VIEWPORT[4] = {0.0, 0.0, 0.33, 1.0};
    double STRAIN_VIEWPORT[4] = {0.33, 0.0, 0.67, 1.0};
    double DEFORMED_SHAPE_VIEWPORT[4] = {0.67, 0.0, 1.0, 1.0};

    const char* STRESS_NAME = "stress";
    const char* STRAIN_NAME = "strain";

public:
    Postprocessor(Preprocessor& preprocessor, Solver& solver);

    ~Postprocessor();

    void run();

    void createGeoemetry(vtkSmartPointer<vtkPolyData> polydata);

    void createDeformedGeoemetry(vtkSmartPointer<vtkPolyData> polydata);

    void createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char name[6]);

    vtkSmartPointer<vtkRenderer> createDataRenderer(std::vector<double> data, double viewport[4], const char name[6]);

    vtkSmartPointer<vtkColorTransferFunction> createColorTransferFunction(double min_value, double max_value);

    void addAxes(vtkSmartPointer<vtkRenderer>& renderer);

    vtkSmartPointer<vtkRenderer> createRenderer(double viewport[4]);

    vtkSmartPointer<vtkRenderer> createDeformedShapeRenderer(double viewport[4]);

    vtkSmartPointer<vtkActor> createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper);

    double calculateScaleFactor(double max_length);

    void createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata);

    void addForces(vtkSmartPointer<vtkRenderer>& renderer);

    vtkSmartPointer<vtkActor> createForceActor(double startPoint[3], double angle);

    double calculateAngle(Force force, char type);
};
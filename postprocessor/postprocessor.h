#include "../solver/Solver.h"
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>


class Postprocessor{
private:
    Preprocessor preprocessor;

    //vtkSmartPointer<vtkPoints> points;
    //vtkSmartPointer<vtkCellArray> lines;
    //vtkSmartPointer<vtkPolyData> polydata;

    const int WINDOW_SIZE = 600;

public:
    Postprocessor(Preprocessor& preprocessor);

    void run();

    void createGeometryObjects();

    vtkSmartPointer<vtkRenderer> createRenderer(std::vector<double> data, double viewport[4]);
};
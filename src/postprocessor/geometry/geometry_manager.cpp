#include "geometry_manager.h"

#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <boost/numeric/ublas/vector.hpp>
#include <memory>

#include "preprocessor/elem_service/ielement.h"
#include "preprocessor/node/node.h"
#include "preprocessor/preprocessor.h"
#include "solver/solver.h"

void geometry::createGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    for (std::shared_ptr<Node> node : preprocessor.getNodes()) {
        points->InsertNextPoint(node->getX(), node->getY(), 0);
    }

    for (std::shared_ptr<IElement> elem : preprocessor.getElements()) {
        std::vector<int> ids = elem->getNodesIndexes();

        int size = ids.size();

        for (int i = 0; i < size; i++) {
            ids[i] = ids[i] - 1;
        }

        std::vector<vtkIdType> cell(ids.begin(), ids.end());
        lines->InsertNextCell(cell.size(), cell.data());
    }

    polydata->SetPoints(points);

    if (lines->GetNumberOfCells() > 0) {
        polydata->SetLines(lines);
    }
}

double geometry::createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata, Preprocessor& preprocessor, Solver& solver) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    double max_length = 0;

    for (std::shared_ptr<IElement> elem : preprocessor.getElements()) {
        std::vector<int> ids = elem->getNodesIndexes();

        int size = ids.size();

        for (int i = 0; i < size; i++) {
            ids[i] = ids[i] - 1;
        }

        std::vector<vtkIdType> cell(ids.begin(), ids.end());
        lines->InsertNextCell(cell.size(), cell.data());

        if (elem->getLength() > max_length) {
            max_length = elem->getLength();
        }
    }

    double scale = calculateScaleFactor(max_length, solver);

    for (std::shared_ptr<Node> node : preprocessor.getNodes()) {
        points->InsertNextPoint(node->getX() + node->getDispX() * scale, node->getY() + node->getDispY() * scale, 0);
    }

    polydata->SetPoints(points);

    if (lines->GetNumberOfCells() > 0) {
        polydata->SetLines(lines);
    }

    return scale;
}

double geometry::calculateScaleFactor(double max_length, Solver& solver) {
    ublas::vector<double> solution = solver.getSolution();

    double max_disp = *std::max_element(solution.begin(), solution.end());
    // double min_disp = *std::min_element(solution.begin(), solution.end());

    // double max_abs_disp = std::max(abs(max_disp), abs(min_disp));

    double ratio = max_disp / max_length;

    double scale = (max_length / 50) / ratio;

    return scale;
}
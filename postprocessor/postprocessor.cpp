#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkColorTransferFunction.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <vtkTransform.h>
#include "postprocessor.h"



Postprocessor::Postprocessor(Preprocessor& preprocessor): preprocessor(preprocessor) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

}

void Postprocessor::run() {
    
    for (Node* node: preprocessor.getNodes()) {
        points->InsertNextPoint(node->getX(), node->getY(), 0);
    }

    for (IElement* elem : preprocessor.getElements()) {
        std::vector<int> ids = elem->getNodesIndexes();

        int size = ids.size();

        for (int i = 0; i < size; i++) {
            ids[i] = ids[i] - 1;
        }

        vtkIdType cell[size] = ids; 
        lines->InsertNextCell(size, cell); 
    }

    polydata->SetPoints(points);

    if (lines.GetNumberOfCells() > 0 ) {
        polydata->SetLines(lines);
    } 


    
}



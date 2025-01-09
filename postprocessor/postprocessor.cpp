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
    
    vtkSmartPointer<vtkFloatArray> stressArray = vtkSmartPointer<vtkFloatArray>::New();
    stressArray->SetName("Stress");
    stressArray->SetNumberOfComponents(1);
}

void Postprocessor::run() {
    
    // Считывание геометрии узлов
    for (Node* node: preprocessor.getNodes()) {
        points->InsertNextPoint(node->getX(), node->getY(), 0);
    }

    for (IElement* elem : preprocessor.getElements()) {
        std::vector<int> ids = elem->getNodesIndexes();

        int size = ids.size();

        for (int i = 0; i < size; i++) {
            ids[i] = ids[i] - 1;
        }

        std::vector<vtkIdType> cell(ids.begin(), ids.end());
        lines->InsertNextCell(cell.size(), cell.data());
    }

    polydata->SetPoints(points);

    if (lines.GetNumberOfCells() > 0) {
        polydata->SetLines(lines);
    }

    // Считывание напряжений
    std::vector<double> stresses;
    for (IElement* elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
    }

    for (double stress : stresses) {
        stressArray->InsertNextValue(stress);
    }
    polydata->GetCellData()->SetScalars(stressArray);

    double minStress = stresses[0];
    double maxStress = stresses[0];
    for (double stress : stresses) {
        minStress = std::min(minStress, stress);
        maxStress = std::max(maxStress, stress);
    }

    vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
    ctf->SetColorSpaceToRGB();

    ctf->AddRGBPoint(minStress, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint((maxStress + minStress) / 2, 0.0, 1.0, 0.0);
    ctf->AddRGBPoint(maxStress/2, 1.0, 1.0, 0.0);        
    ctf->AddRGBPoint(maxStress, 1.0, 0.0, 0.0); 

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);
    mapper->SetLookupTable(ctf);
    mapper->SetScalarRange(minStress, maxStress);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(3.0); 

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkNew<vtkTransform> transform;
    transform->Translate(0.0, 0.0, 0.0);

    vtkNew<vtkAxesActor> axes;

    axes->SetUserTransform(transform);

    renderer->AddActor(axes);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(600, 600);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();

    return 0;



}



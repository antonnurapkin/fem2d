#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkColorTransferFunction.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkTransform.h>
#include "postprocessor.h"


Postprocessor::Postprocessor(Preprocessor& preprocessor): preprocessor(preprocessor) {
    this->points = vtkSmartPointer<vtkPoints>::New();
    this->lines = vtkSmartPointer<vtkCellArray>::New();
    this->polydata = vtkSmartPointer<vtkPolyData>::New();
    
    this->stressArray = vtkSmartPointer<vtkFloatArray>::New();
    this->stressArray->SetName("Stress");
    this->stressArray->SetNumberOfComponents(1);
}

void Postprocessor::run() {
    
    createGeometryObjects();

    std::vector<double> stresses;
    for (IElement* elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
    }

    for (double stress : stresses) {
        this->stressArray->InsertNextValue(stress);
    }
    this->polydata->GetCellData()->SetScalars(this->stressArray);

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
    mapper->SetInputData(this->polydata);
    mapper->SetLookupTable(ctf);
    mapper->SetScalarRange(minStress, maxStress);

    renderWindow(mapper);
}

void Postprocessor::createGeometryObjects(){
    for (Node* node: preprocessor.getNodes()) {
        this->points->InsertNextPoint(node->getX(), node->getY(), 0);
    }

    for (IElement* elem : preprocessor.getElements()) {
        std::vector<int> ids = elem->getNodesIndexes();

        int size = ids.size();

        for (int i = 0; i < size; i++) {
            ids[i] = ids[i] - 1;
        }

        std::vector<vtkIdType> cell(ids.begin(), ids.end());
        this->lines->InsertNextCell(cell.size(), cell.data());
    }

    this->polydata->SetPoints(points);

    if (this->lines->GetNumberOfCells() > 0) {
        this->polydata->SetLines(this->lines);
    }
}

void Postprocessor::renderWindow(vtkSmartPointer<vtkPolyDataMapper> mapper) {
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
    renderWindow->SetSize(WINDOW_SIZE, WINDOW_SIZE);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();
}



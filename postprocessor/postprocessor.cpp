#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
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
    // this->points = vtkSmartPointer<vtkPoints>::New();
    // this->lines = vtkSmartPointer<vtkCellArray>::New();

    // this->polydata = vtkSmartPointer<vtkPolyData>::New();
}

void Postprocessor::run() {

    std::vector<double> stresses;
    std::vector<double> strains;

    for (IElement* elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
        strains.push_back(elem->getStrain());
    }

    double stress_viewport[4] = {0.0, 0.0, 0.5, 1.0};
    double strain_viewport[4] = {0.5, 0.0, 1.0, 1.0};

    vtkSmartPointer<vtkRenderer> stress_renderer = createRenderer(stresses, stress_viewport);
    vtkSmartPointer<vtkRenderer> strain_renderer = createRenderer(strains, strain_viewport); 

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(stress_renderer);
    renderWindow->AddRenderer(strain_renderer);
    renderWindow->SetSize(2 * WINDOW_SIZE, WINDOW_SIZE);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();
}

void Postprocessor::createGeometryObjects(){
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for (Node* node: preprocessor.getNodes()) {
        points->InsertNextPoint(node->getX(), node->getY(), 0);
    }

    // for (IElement* elem : preprocessor.getElements()) {
    //     std::vector<int> ids = elem->getNodesIndexes();

    //     int size = ids.size();

    //     for (int i = 0; i < size; i++) {
    //         ids[i] = ids[i] - 1;
    //     }

    //     std::vector<vtkIdType> cell(ids.begin(), ids.end());
    //     lines->InsertNextCell(cell.size(), cell.data());
    // }
}

vtkSmartPointer<vtkRenderer> Postprocessor::createRenderer(std::vector<double> data, double viewport[4]) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

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

    if (lines->GetNumberOfCells() > 0) {
        polydata->SetLines(lines);
    }

    vtkSmartPointer<vtkFloatArray> data_store = vtkSmartPointer<vtkFloatArray>::New();
    data_store->SetNumberOfComponents(1);

    for (double value : data) {
        data_store->InsertNextValue(value);
    }
    polydata->GetCellData()->SetScalars(data_store);

    double min_value = *max_element(data.begin(), data.end());
    double max_value = *min_element(data.begin(), data.end());

    vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
    ctf->SetColorSpaceToRGB();

    ctf->AddRGBPoint(min_value, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint((max_value + min_value) / 2, 0.0, 1.0, 0.0);
    ctf->AddRGBPoint(max_value / 2, 1.0, 1.0, 0.0);        
    ctf->AddRGBPoint(max_value, 1.0, 0.0, 0.0); 

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);
    mapper->SetLookupTable(ctf);
    mapper->SetScalarRange(min_value, max_value);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(3.0);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetViewport(viewport);
    renderer->ResetCamera();
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkNew<vtkTransform> transform;
    transform->Translate(0.0, 0.0, 0.0);

    vtkNew<vtkAxesActor> axes;

    axes->SetUserTransform(transform);

    renderer->AddActor(axes);

    return renderer;
}



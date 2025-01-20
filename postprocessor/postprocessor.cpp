#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkTransform.h>
#include <vtkTextProperty.h>
#include <vtkScalarBarActor.h>
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

    const char* stress_name = "stress";
    const char* strain_name = "strain";

    vtkSmartPointer<vtkRenderer> stress_renderer = createDataRenderer(stresses, stress_viewport, stress_name);
    vtkSmartPointer<vtkRenderer> strain_renderer = createDataRenderer(strains, strain_viewport, strain_name); 

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(stress_renderer);
    renderWindow->AddRenderer(strain_renderer);
    renderWindow->SetSize(2 * WINDOW_SIZE, WINDOW_SIZE);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();

    delete stress_name, strain_name;
}

vtkSmartPointer<vtkRenderer> Postprocessor::createDataRenderer(std::vector<double> data, double viewport[4], const char * name) {
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();

    createGeoemetry(polydata);

    vtkSmartPointer<vtkFloatArray> data_store = vtkSmartPointer<vtkFloatArray>::New();
    data_store->SetNumberOfComponents(1);

    for (double value : data) {
        data_store->InsertNextValue(value);
    }
    polydata->GetCellData()->SetScalars(data_store);

    double min_value = *min_element(data.begin(), data.end());
    double max_value = *max_element(data.begin(), data.end());

    vtkSmartPointer<vtkColorTransferFunction> ctf = createColorTransferFunction(min_value, max_value);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);
    mapper->SetLookupTable(ctf);
    mapper->SetScalarRange(min_value, max_value);

    vtkSmartPointer<vtkActor> actor = createActor(mapper);
    vtkSmartPointer<vtkRenderer> renderer = createRenderer(viewport);

    renderer->AddActor(actor);
    createScalarBar(renderer, mapper->GetLookupTable(), name);
    addAxes(renderer);

    return renderer;
}

void Postprocessor::createGeoemetry(vtkSmartPointer<vtkPolyData> polydata) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

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
}

void Postprocessor::createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char * name) {
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(lookupTable);
    scalarBar->SetTitle(name); // Используем имя, переданное в функцию, как заголовок
    scalarBar->SetNumberOfLabels(8);

    scalarBar->SetPosition(0.05, 0.05); //  Левый нижний угол, отступ 5%
    scalarBar->SetPosition2(0.05, 0.9); // Вертикальная полоса
    scalarBar->GetLabelTextProperty()->SetColor(0, 0, 0); // Установить цвет текста меток в черный
    scalarBar->GetTitleTextProperty()->SetColor(0, 0, 0); // Установить цвет заголовка в черный
    scalarBar->SetUnconstrainedFontSize(24);

    renderer->AddActor2D(scalarBar);
}

vtkSmartPointer<vtkColorTransferFunction> Postprocessor::createColorTransferFunction(double min_value, double max_value){
    vtkSmartPointer<vtkColorTransferFunction> ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
    ctf->SetColorSpaceToRGB();

    double step = (max_value - min_value) / 4;

    ctf->AddRGBPoint(min_value, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint(min_value + step, 0, 1.0, 1.0);
    ctf->AddRGBPoint(min_value + 2 * step, 0.0, 1.0, 0.0);
    ctf->AddRGBPoint(max_value - step, 1.0, 1.0, 0);
    ctf->AddRGBPoint(max_value, 1.0, 0.0, 0.0);

    return ctf;
}

void Postprocessor::addAxes(vtkSmartPointer<vtkRenderer>& renderer) {
    vtkNew<vtkTransform> transform;
    transform->Translate(0.0, 0.0, 0.0);

    vtkNew<vtkAxesActor> axes;
    axes->SetUserTransform(transform);
    renderer->AddActor(axes);
}

vtkSmartPointer<vtkRenderer> Postprocessor::createRenderer(double viewport[4]) {
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetViewport(viewport);
    renderer->ResetCamera();
    renderer->SetBackground(1.0, 1.0, 1.0);
    return renderer;
}

vtkSmartPointer<vtkActor> Postprocessor::createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper) {
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(3.0);
    return actor;
}
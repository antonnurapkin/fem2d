#include <vtkFloatArray.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
#include <vtkScalarBarActor.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>

#include "geometry/geometry_manager.h"
#include "boundaries/boundary_conditions_manager.h"
#include "renderer_manager.h"

vtkSmartPointer<vtkRenderer> RendererManager::createDataRenderer(vtkSmartPointer<vtkPolyData> polydata, std::vector<double> data, const double viewport[4], const char * name, Preprocessor& preprocessor) {
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
    boundaryConditionsManager.addForces(renderer, preprocessor);
    boundaryConditionsManager.addSupports(renderer, preprocessor);

    return renderer;
}

vtkSmartPointer<vtkRenderer> RendererManager::createDeformedShapeRenderer(double scale, vtkSmartPointer<vtkPolyData> polydataOriginal, vtkSmartPointer<vtkPolyData> polydataDeformed, const double viewport[4], Preprocessor& preprocessor) {

    vtkSmartPointer<vtkPolyDataMapper> mapperOriginal = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperOriginal->SetInputData(polydataOriginal);

    vtkSmartPointer<vtkPolyDataMapper> mapperDeformed = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperDeformed->SetInputData(polydataDeformed);

    vtkSmartPointer<vtkActor> actorOriginal = createActor(mapperOriginal);
    actorOriginal->GetProperty()->SetColor(const_cast<double*>(BLACK));
    actorOriginal->GetProperty()->SetOpacity(0.3);

    vtkSmartPointer<vtkActor> actorDeformed = createActor(mapperDeformed);
    actorDeformed->GetProperty()->SetColor(const_cast<double*>(BLACK));

    vtkSmartPointer<vtkRenderer> renderer = createRenderer(viewport);
    renderer->AddActor(actorOriginal);
    renderer->AddActor(actorDeformed);

    addAxes(renderer);
    boundaryConditionsManager.addSupports(renderer, preprocessor);
    boundaryConditionsManager.addForces(renderer, preprocessor, scale);

    return renderer;
}

void RendererManager::addAxes(vtkSmartPointer<vtkRenderer>& renderer) {
    vtkNew<vtkTransform> transform;
    transform->Translate(0.0, 0.0, 0.0);

    vtkNew<vtkAxesActor> axes;
    axes->SetUserTransform(transform);
    renderer->AddActor(axes);
}

vtkSmartPointer<vtkRenderer> RendererManager::createRenderer(const double viewport[4]) {
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetViewport(viewport);
    renderer->ResetCamera();
    renderer->SetBackground(1.0, 1.0, 1.0);
    return renderer;
}

vtkSmartPointer<vtkActor> RendererManager::createActor(vtkSmartPointer<vtkPolyDataMapper>& mapper) {
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(LINE_WIDTH);
    return actor;
}

void RendererManager::createScalarBar(vtkSmartPointer<vtkRenderer> renderer, vtkScalarsToColors* lookupTable, const char * name) {
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

vtkSmartPointer<vtkColorTransferFunction> RendererManager::createColorTransferFunction(double min_value, double max_value){
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
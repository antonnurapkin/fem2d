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
#include <vtkArrowSource.h>
#include <vtkMath.h>
#include <vtkTransformPolyDataFilter.h>
#include "postprocessor.h"


Postprocessor::Postprocessor(Preprocessor& preprocessor, Solver& solver): preprocessor(preprocessor), solver(solver) {}; 

Postprocessor::~Postprocessor() {
    delete STRESS_NAME, STRAIN_NAME;
}

void Postprocessor::run() {

    std::vector<double> stresses;
    std::vector<double> strains;

    for (IElement* elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
        strains.push_back(elem->getStrain());
    }

    vtkSmartPointer<vtkRenderer> stress_renderer = createDataRenderer(stresses, STRESS_VIEWPORT, STRESS_NAME);
    vtkSmartPointer<vtkRenderer> strain_renderer = createDataRenderer(strains, STRAIN_VIEWPORT, STRAIN_NAME);
    vtkSmartPointer<vtkRenderer> deformed_shape_renderer = createDeformedShapeRenderer(DEFORMED_SHAPE_VIEWPORT); 

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(stress_renderer);
    renderWindow->AddRenderer(strain_renderer);
    renderWindow->AddRenderer(deformed_shape_renderer);

    renderWindow->SetSize(3 * WINDOW_SIZE, WINDOW_SIZE);

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    interactor->Start();
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
    addForces(renderer);

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
    actor->GetProperty()->SetLineWidth(LINE_WIDTH);
    return actor;
}

vtkSmartPointer<vtkRenderer> Postprocessor::createDeformedShapeRenderer(double viewport[4]) {
    vtkSmartPointer<vtkPolyData> polydataOriginal = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> polydataDeformed = vtkSmartPointer<vtkPolyData>::New();

    createGeoemetry(polydataOriginal);

    createDeformedGeometry(polydataDeformed);

    vtkSmartPointer<vtkPolyDataMapper> mapperOriginal = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperOriginal->SetInputData(polydataOriginal);

    vtkSmartPointer<vtkPolyDataMapper> mapperDeformed = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperDeformed->SetInputData(polydataDeformed);

    double black[3] = {0, 0, 0};

    vtkSmartPointer<vtkActor> actorOriginal = createActor(mapperOriginal);
    actorOriginal->GetProperty()->SetColor(black);
    actorOriginal->GetProperty()->SetOpacity(0.3);

    vtkSmartPointer<vtkActor> actorDeformed = createActor(mapperDeformed);
    actorDeformed->GetProperty()->SetColor(black);

    vtkSmartPointer<vtkRenderer> renderer = createRenderer(viewport);
    renderer->AddActor(actorOriginal);
    renderer->AddActor(actorDeformed);

    addAxes(renderer);

    return renderer;
}

void Postprocessor::createDeformedGeometry(vtkSmartPointer<vtkPolyData> polydata) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

    double max_length = 0;

    for (IElement* elem : preprocessor.getElements()) {
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

    double scale = calculateScaleFactor(max_length);

    for (Node* node: preprocessor.getNodes()) {
        points->InsertNextPoint(
            node->getX() + node->getDispX() * scale, 
            node->getY() + node->getDispY() * scale, 
            0
        );
    }

    polydata->SetPoints(points);

    if (lines->GetNumberOfCells() > 0) {
        polydata->SetLines(lines);
    }
}

double Postprocessor::calculateScaleFactor(double max_length) {
    boost::numeric::ublas::vector<double> solution = solver.getSolution();

    double max_disp = *max_element(solution.begin(), solution.end());
    double min_disp = *min_element(solution.begin(), solution.end());

    double max_abs_disp = std::max(abs(max_disp), abs(min_disp));

    double ratio = max_disp / max_length;

    double scale = (max_length / 50) / ratio;

    return scale;

}

void Postprocessor::addForces(vtkSmartPointer<vtkRenderer>& renderer) {
    for(const auto& force: preprocessor.getForces()) {
        Node* node = preprocessor.getNodeByIndex(force.getIndex());

        double startPoint[3] = {node->getX(), node->getY(), 0.0};

        if (force.getForceX() != 0) {
            double angle = calculateAngle(force, 'x');
            vtkSmartPointer<vtkActor> actor = createForceActor(startPoint, angle);
            renderer->AddActor(actor);
        }
        if (force.getForceY() != 0) {
            double angle = calculateAngle(force, 'y');
            vtkSmartPointer<vtkActor> actor = createForceActor(startPoint, angle);
            renderer->AddActor(actor);
        }
    }
}

vtkSmartPointer<vtkActor> Postprocessor::createForceActor(double startPoint[3], double angle) {

    auto arrowSource = vtkSmartPointer<vtkArrowSource>::New();
    arrowSource->SetShaftRadius(LINE_WIDTH * 0.005);
    arrowSource->SetTipRadius(LINE_WIDTH * 0.02);

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(startPoint);
    transform->RotateZ(angle);

    auto transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetTransform(transform);
	transformFilter->SetInputConnection(arrowSource->GetOutputPort());

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(transformFilter->GetOutputPort());
	
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    return actor;
}

double Postprocessor::calculateAngle(Force force, char type) {
    if (type == 'x' and force.getForceX() > 0) {
        return 0;
    } 
    else if (type == 'x' and force.getForceX() < 0) {
        return 180;
    }
    else if (type == 'y' and force.getForceY() > 0) {
        return 90;
    } 
    else if (type == 'y' and force.getForceY() < 0) {
        return -90;
    }
}
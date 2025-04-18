#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "preprocessor/preprocessor.h"
#include "preprocessor/elem_service/ielement.h"
#include "solver/solver.h"
#include "postprocessor.h"
#include "geometry/geometry_manager.h"
#include "vizualization_params.h"


Postprocessor::Postprocessor(Solver& solver): solver_(solver) {
    std::cout << "Postprocessor was created successfully!\n";
};

Postprocessor::~Postprocessor() {
    std::cout << "Postprocessor was deleted successfully!\n";
}

void Postprocessor::run() {

    Preprocessor& preprocessor = solver_.getPreprocessor();

    std::vector<double> stresses, strains;

    for (std::shared_ptr<IElement> elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
        strains.push_back(elem->getStrain());
    }
    vtkSmartPointer<vtkPolyData> polydataOriginalStrain = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> polydataOriginalStress = vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkPolyData> polydataOriginal = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> polydataDeformed = vtkSmartPointer<vtkPolyData>::New();

    geometry::createGeometry(polydataOriginalStrain, preprocessor);
    geometry::createGeometry(polydataOriginalStress, preprocessor);

    geometry::createGeometry(polydataOriginal, preprocessor);
    double scale = geometry::createDeformedGeometry(polydataDeformed, preprocessor, solver_);
    
    vtkSmartPointer<vtkRenderer> stress_renderer = rendererManager_.createDataRenderer(
        polydataOriginalStress, 
        stresses, 
        STRESS_VIEWPORT, 
        STRESS_NAME,
        preprocessor
    );
    vtkSmartPointer<vtkRenderer> strain_renderer = rendererManager_.createDataRenderer(
        polydataOriginalStrain, 
        strains, 
        STRAIN_VIEWPORT, 
        STRAIN_NAME, 
        preprocessor
    );
    vtkSmartPointer<vtkRenderer> deformed_shape_renderer = rendererManager_.createDeformedShapeRenderer(
        scale,
        polydataOriginal, 
        polydataDeformed, 
        DEFORMED_SHAPE_VIEWPORT, 
        preprocessor
    ); 

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
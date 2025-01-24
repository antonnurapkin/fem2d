#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include "vizualizationParams.h"
#include "../preprocessor/preprocessor.h"
#include "../solver/solver.h"

#include "postprocessor.h"

Postprocessor::Postprocessor(Preprocessor& preprocessor, Solver& solver): preprocessor(preprocessor), solver(solver) {};

void Postprocessor::run() {

    std::vector<double> stresses, strains;

    for (IElement* elem : preprocessor.getElements()) {
        stresses.push_back(elem->getStress());
        strains.push_back(elem->getStrain());
    }
    vtkSmartPointer<vtkPolyData> polydataOriginalStrain = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> polydataOriginalStress = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyData> polydataDeformed = vtkSmartPointer<vtkPolyData>::New();

    // TODO: Уменьшить количетсов polydata
    rendererManager.geometryManager.createGeometry(polydataOriginalStrain, preprocessor);
    rendererManager.geometryManager.createGeometry(polydataOriginalStress, preprocessor);
    rendererManager.geometryManager.createDeformedGeometry(polydataDeformed, preprocessor, solver);
    
    vtkSmartPointer<vtkRenderer> stress_renderer = rendererManager.createDataRenderer(
        polydataOriginalStress, 
        stresses, 
        STRESS_VIEWPORT, 
        STRESS_NAME,
        preprocessor
    );
    vtkSmartPointer<vtkRenderer> strain_renderer = rendererManager.createDataRenderer(
        polydataOriginalStrain, 
        strains, 
        STRAIN_VIEWPORT, 
        STRAIN_NAME, 
        preprocessor
    );
    vtkSmartPointer<vtkRenderer> deformed_shape_renderer = rendererManager.createDeformedShapeRenderer(
        polydataOriginalStrain, 
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
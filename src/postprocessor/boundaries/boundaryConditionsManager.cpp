#include <memory>
#include <vtkArrowSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkConeSource.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkTransformFilter.h>
 
#include "../preprocessor/preprocessor.h"
#include "../vizualizationParams.h"
#include "boundaryConditionsManager.h"

void BoundaryConditionsManager::addForces(vtkSmartPointer<vtkRenderer>& renderer, Preprocessor& preprocessor, double scale){
    for(const auto& force: preprocessor.getForces()) {
        std::shared_ptr<Node> node = preprocessor.getNodeByIndex(force.getIndex());

        double startPoint[3] = {node->getX() + node->getDispX() * scale, node->getY() + node->getDispY() * scale, 0.0};

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

vtkSmartPointer<vtkActor> BoundaryConditionsManager::createForceActor(double startPoint[3], double angle) {

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
    actor->GetProperty()->SetColor(const_cast<double*>(BLACK));

    return actor;
}

double BoundaryConditionsManager::calculateAngle(Force force, char type) {
    if (type == 'x' && force.getForceX() > 0) {
        return 0;
    } 
    else if (type == 'x' && force.getForceX() < 0) {
        return 180;
    }
    else if (type == 'y' && force.getForceY() > 0) {
        return 90;
    } 
    else if (type == 'y' && force.getForceY() < 0) {
        return -90;
    }
}

void BoundaryConditionsManager::addSupports(vtkSmartPointer<vtkRenderer>& renderer, Preprocessor& preprocessor) {
    for(const auto& support: preprocessor.getSupports()) {
        std::shared_ptr<Node> node = preprocessor.getNodeByIndex(support.getIndex());

        double startPoint[3] = {node->getX(), node->getY(), 0.0};

        if (support.getDispX() == 0) {
            vtkSmartPointer<vtkActor> actor = createSupportActor(startPoint, true);
            renderer->AddActor(actor);
        }
        if (support.getDispY() == 0) {
            vtkSmartPointer<vtkActor> actor = createSupportActor(startPoint, false);
            renderer->AddActor(actor);
        }
    }
}

vtkSmartPointer<vtkActor> BoundaryConditionsManager::createSupportActor(double startPoint[3], bool isXAxis) {
    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetHeight(LINE_WIDTH * 0.1); // Высота конуса
    coneSource->SetRadius(LINE_WIDTH * 0.04); // Радиус основания конуса
    coneSource->SetResolution(50); // Разрешение конуса

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

    transform->Translate(startPoint[0], startPoint[1], startPoint[2]);
    if (isXAxis) {
        transform->RotateZ(0);
    } else {
        transform->RotateZ(90);
    }
    
    transform->Translate(-coneSource->GetHeight() / 2, 0, 0);

    vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
    transformFilter->SetInputConnection(coneSource->GetOutputPort());
    transformFilter->SetTransform(transform);

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(transformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(const_cast<double*>(BLACK));

    return coneActor;
}


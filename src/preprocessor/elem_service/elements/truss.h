#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <memory>
#include <string>
#include <vector>

#include "elem_service/elem_params.h"
#include "elem_service/ielement.h"
#include "material/material.h"
#include "node/node.h"

class Truss final : public IElement {
   public:
    Truss(Material material, std::vector<int> indexes, std::vector<std::shared_ptr<Node>> nodes, double section);
    Truss(ElemParams& elemParams);

    ublas::matrix<double> KMatrixElemGlobal() const override;
    ublas::matrix<double> KMatrixElemLocal() const;
    ublas::matrix<double> BMatrix() const override;
    ublas::matrix<double> DMatrix() const override;

    ublas::matrix<double> getDisplacments() const;

    double getStrain() const override;
    double getStress() const override;

    std::vector<std::shared_ptr<Node>> getNodes() const override;

    std::vector<int> getNodesIndexes() const override;

    double getLength() const override;

    double setLength(std::vector<std::shared_ptr<Node>> nodes) const;
    double setAngle(std::vector<std::shared_ptr<Node>> nodes) const;

   private:
    std::vector<std::shared_ptr<Node>> nodes;
    Material material;
    std::vector<int> indexes;
    double section;
    double length;
    double angle;

    int MATRIX_SIZE = 4;
};

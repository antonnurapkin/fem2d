#pragma once
#include <vector>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "../../material/Material.h"
#include "../IElement.h"
#include "../ElemParams.h"

class Truss : public IElement
{
private:
	std::vector<Node*> nodes;
	Material material;
	std::vector<int> indexes;
	double section;
	double length;
	double angle;
	int MATRIX_SIZE = 4;
public:
	Truss(Material material, std::vector<int> indexes, std::vector<Node*> nodes, double section);
	Truss(ElemParams& elemParams);

	ublas::matrix<double> KMatrixElemGlobal() const override;
	ublas::matrix<double> KMatrixElemLocal() const;
	ublas::matrix<double> BMatrix() const override;
	ublas::matrix<double> DMatrix() const override;

	ublas::matrix<double> getDisplacments() const;

	double getStrain() const;
	double getStress() const;

	std::vector<Node*> getNodes() const override;
	std::vector<int> getNodesIndexes() const;

	double getLength() const override;
	
	double setLength(std::vector<Node*> nodes) const;
	double setAngle(std::vector<Node*> nodes) const;	
};


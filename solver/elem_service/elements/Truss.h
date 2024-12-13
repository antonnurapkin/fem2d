#pragma once
#include <vector>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include "Material.h"
#include "IElement.h"
#include "ElemParams.h"

class Truss : public IElement
{
private:
	std::vector<Node> nodes;
	Material material;
	std::vector<int> indexes;
	double section;
	double length;
	double angle;
	int matrix_size = 4;

	double setLength(std::vector<Node> nodes) const;
	double setAngle(std::vector<Node> nodes) const;
public:
	Truss(Material material, std::vector<int> indexes, std::vector<Node> nodes, double section);
	Truss(ElemParams& elemParams);

	ublas::matrix<double> KMatrixElemGlobal() const override;
	ublas::matrix<double> KMatrixElemLocal() const;
	ublas::matrix<double> BMatrix() const override;
	ublas::matrix<double> DMatrix() const override;

	std::vector<Node> getNodes() const;
	std::vector<int> getNodesIndexes() const;
	/*ublas::vector<double> getStrain() const;
	ublas::vector<double> getStress() const;*/

	double getLength();
};


#pragma once
#include <vector>
#include <memory>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "node/node.h"

using namespace boost::numeric;

class IElement {
public:
	virtual ublas::matrix<double> KMatrixElemGlobal() const = 0;
	virtual ublas::matrix<double> BMatrix() const = 0;
	virtual ublas::matrix<double> DMatrix() const = 0;

	virtual std::vector<std::shared_ptr<Node>> getNodes() const = 0;
	virtual std::vector<int> getNodesIndexes() const = 0;

	virtual double getStrain() const = 0;
	virtual double getStress() const = 0;
	virtual double getLength() const = 0;
};

#include <cmath>
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Truss.h"
#include "../../node/Node.h"
#include "../ElemParams.h"
#include "../../utils/tools.h"

using namespace boost::numeric;

Truss::Truss(Material material, std::vector<int> indexes, std::vector<Node*> nodes, double section)
	:material(material), indexes(indexes), nodes(nodes), section(section) {
	length = setLength(nodes);
	angle = setAngle(nodes);
}

Truss::Truss(ElemParams& elem_params)
	: material(elem_params.material), indexes(elem_params.indexes), nodes(elem_params.nodes), section(elem_params.geom_parameter)
{
	length = setLength(nodes);
	angle = setAngle(nodes);
}

ublas::matrix<double> Truss::KMatrixElemGlobal() const {
	ublas::matrix<double> K_elem_local = KMatrixElemLocal();

	ublas::matrix<double> T_matrix = TransformMatrix(angle);

	ublas::matrix<double> K_elem_global_temp = ublas::prod(K_elem_local, T_matrix);

	ublas::matrix<double> K_elem_global = ublas::prod(ublas::trans(T_matrix), K_elem_global_temp);

	return K_elem_global;
}

ublas::matrix<double> Truss::KMatrixElemLocal() const
{
	ublas::matrix<double> K_elem_local(MATRIX_SIZE, MATRIX_SIZE);

	K_elem_local = (material.getEmod() * section * length) * ublas::prod(BMatrix(), ublas::trans(BMatrix()));
	return K_elem_local;
}

ublas::matrix<double> Truss::BMatrix() const {
	ublas::matrix<double> B(MATRIX_SIZE, 1);

	B <<= -1 / length,
		  0,
		  1 / length,
		  0;

	return B;
}

ublas::matrix<double> Truss::DMatrix() const
{
	return ublas::identity_matrix<double>(MATRIX_SIZE, MATRIX_SIZE) * material.getEmod();
}

std::vector<Node*> Truss::getNodes() const
{
	return nodes;
}

std::vector<int> Truss::getNodesIndexes() const
{
	return indexes;
}

ublas::matrix<double> Truss::getDisplacments() const
{

	ublas::matrix<double> displacements(MATRIX_SIZE, 1);	

	for(int i = 0; i < nodes.size(); i++) {
		displacements(2 * i) = nodes[i]->getDispX();
		displacements(2 * i + 1) = nodes[i]->getDispY();
	}

    return displacements;
}

double Truss::getStrain() const
{	
	ublas::matrix<double> disp = getDisplacments();

	std::vector<Node*> nodes = getNodes();

	double x1_new = nodes[0]->getX() + disp(0);
	double x2_new = nodes[1]->getX() + disp(2);

	double y1_new = nodes[0]->getY() + disp(1);
	double y2_new = nodes[1]->getY() + disp(3);

	double new_length = pow(pow(x1_new - x2_new, 2) + pow(y1_new - y2_new, 2), 0.5);

	double strain = (new_length - length) / length;
    return strain;
}

double Truss::getStress() const
{	
	double stress = this->material.getEmod() * this->getStrain();

    return stress;
}

double Truss::setLength(std::vector<Node*> nodes) const
{
	double x1 = nodes[0]->getX();
	double x2 = nodes[1]->getX();

	double y1 = nodes[0]->getY();
	double y2 = nodes[1]->getY();

	return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

double Truss::setAngle(std::vector<Node*> nodes) const
{
	double x1 = nodes[0]->getX();
	double x2 = nodes[1]->getX();

	double y1 = nodes[0]->getY();
	double y2 = nodes[1]->getY();

	return atan((y2 - y1) / (x2 - x1));
}

double Truss::getLength()
{
	return length;
}









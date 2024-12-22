#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/assignment.hpp>
#include "Truss.h"
#include "../../node/Node.h"
#include "../../material/Material.h"
#include "../ElemParams.h"
#include "../../utils/tools.h"

using namespace boost::numeric;

Truss::Truss(Material material, std::vector<int> indexes, std::vector<Node> nodes, double section)
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

	/*for (int i = 0; i < indexes.size(); i++) {
		std::cout << indexes[i] << " ";
	}
	std::cout << std::endl;
	std::cout << angle / 3.1416 * 180 << std::endl;
	
	ublas::matrix<double> K_elem_normalized = K_elem_global / (material.getEmod() * section) * length;
	print_matrix(K_elem_normalized);*/

	return K_elem_global;
}

ublas::matrix<double> Truss::KMatrixElemLocal() const
{
	ublas::matrix<double> K_elem_local(matrix_size, matrix_size);

	K_elem_local = (material.getEmod() * section * length) * ublas::prod(BMatrix(), ublas::trans(BMatrix()));
	return K_elem_local;
}

ublas::matrix<double> Truss::BMatrix() const {
	ublas::matrix<double> B(matrix_size, 1);

	B <<= -1 / length,
		  0,
		  1 / length,
		  0;

	return B;
}

ublas::matrix<double> Truss::DMatrix() const
{
	return ublas::identity_matrix<double>(matrix_size, matrix_size) * material.getEmod();
}

std::vector<Node> Truss::getNodes() const
{
	return nodes;
}

std::vector<int> Truss::getNodesIndexes() const
{
	return indexes;
}

double Truss::setLength(std::vector<Node> nodes) const
{
	double x1 = nodes[0].getX();
	double x2 = nodes[1].getX();

	double y1 = nodes[0].getY();
	double y2 = nodes[1].getY();

	return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

double Truss::setAngle(std::vector<Node> nodes) const
{
	double x1 = nodes[0].getX();
	double x2 = nodes[1].getX();

	double y1 = nodes[0].getY();
	double y2 = nodes[1].getY();

	return atan((y2 - y1) / (x2 - x1));
}

double Truss::getLength()
{
	return length;
}









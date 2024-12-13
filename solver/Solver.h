#pragma once
#include<map>
#include <vector>
#include <array>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "IElement.h"
#include "Node.h"
#include "Material.h"
#include "Force.h"
#include "Support.h"
#include "ElemParams.h"


class Solver
{
private:
	std::vector<IElement*> elements;
	std::vector<Node> nodes;
	std::vector<Material> materials;
	std::vector<Force> forces;
	std::vector<Support> supports;

	std::array<std::string, 8> commands = { "ETYPE", "MATERIAL", "SECTION", "THICKNESS", "NODE", "ELEM", "DISP", "FORCE" };
public:
	Solver(std::string config);
	~Solver();
	void run(std::string config);

	ublas::matrix<double> createKGlobal(int matrix_size);
	ublas::vector<double> createFGlobal(int vector_size);

	ublas::matrix<double> assembleMatrices(ublas::matrix<double>& Klocal, ublas::matrix<double>& Kglobal, IElement* elem);
	ublas::matrix<double> applySupports(ublas::matrix<double>& Kglobal, int matrix_size);
	
	void readConfig(std::string config);
	int calculateMatrixSize();

	ElemParams createElemParams(std::map<std::string, std::optional<double>> elem_data, double geometry);
	Node getNodeByIndex(int index);
	Material getMaterialByIndex(int index);
	std::vector<int> getDofIndexes(Support support);
};


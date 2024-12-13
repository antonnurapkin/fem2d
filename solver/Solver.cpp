#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Solver.h"
#include "material/Material.h"
#include "elem_service/ElemCreator.h"
#include "elem_service/ElemParams.h"
#include "boundaries/Force.h"
#include "elem_service/IElement.h"
#include "utils/Error.h"
#include "utils/tools.h"


Solver::Solver(std::string config)
{
	std::cout << "Solver was created\n";
	run(config);
}

Solver::~Solver() {
	for (auto* elem : elements) {
		if (elem != nullptr) {
			delete elem;
		}
	}
}

void Solver::run(std::string config) {
	readConfig(config);

	int matrix_size = calculateMatrixSize();

	ublas::matrix<double> Kglobal = createKGlobal(matrix_size);

	ublas::vector<double> Fvector = createFGlobal(matrix_size);

	ublas::vector<double> dispSolution = solveSystem(Kglobal, Fvector);

	std::cout << "Solution:\n";
	std::cout << dispSolution;
}

void Solver::readConfig(std::string config) {
	try {
		std::fstream file(config);
		if (file.is_open()) {
			std::string line;
			std::string etype;
			double section;

			while (std::getline(file, line)) {

				if (line.find("ETYPE") != std::string::npos && etype.size() == 0) {
					etype = getElementType(line);
				}

				else if (line.find("MATERIAL") != std::string::npos) {
					std::map<std::string, std::optional<double>> mat_props = getDataFromString(line, { "Emod", "mu", "density", "index" });
					Material material(mat_props["Emod"], mat_props["mu"], mat_props["density"], mat_props["index"]);
					this->materials.push_back(material);
				}

				else if (line.find("SECTION") != std::string::npos) {
					section = getSection(line);
				}

				else if (line.find("NODE") != std::string::npos) {
					std::map<std::string, std::optional<double>> node_data = getDataFromString(line, { "index", "x", "y" });
					Node node(node_data["index"], node_data["x"], node_data["y"]);
					this->nodes.push_back(node);
				}
				// TODO: ������� �������� ������ ���������� � ����������� �� ���� ���������
				else if (line.find("ELEM") != std::string::npos) {
					std::map<std::string, std::optional<double>> elem_data = getDataFromString(line, { "index1", "index2", "material_index" });

					ElemParams elem_params = createElemParams(elem_data, section);

					IElement* elem = ElemCreator::getElement(etype, elem_params);
					elements.push_back(elem);
				}

				else if (line.find("FORCE") != std::string::npos) {
					std::map<std::string, std::optional<double>> force_components = getDataFromString(line, { "index", "Fx", "Fy" });
					Force force(force_components["index"], force_components["Fx"], force_components["Fy"]);
					this->forces.push_back(force);
				}

				else if (line.find("DISP") != std::string::npos) {
					std::map<std::string, std::optional<double>> support_components = getDataFromString(line, { "index", "disp_x", "disp_y" });
					Support support(support_components["index"], support_components["disp_x"], support_components["disp_y"]);
					this->supports.push_back(support);
				}
			}

			std::cout << "Configuration file was readed successfully!\n";
			std::fstream file(config);
		}
		else {
			throw Error("The config file does not exist in this path\n");
		}
	}
	catch (const Error& err) {
		std::cout << "\nError while config file reading:\n" << err.what() << std::endl;
		std::exit(1);
	}
}

//TODO: �������� ��� ������-��������
Node Solver::getNodeByIndex(int index) {
	for (Node node : nodes) {
		if (node.getIndex() == index) {
			return node;
		}
	}
	throw Error("Node with the specified index does not exist\n");
}

Material Solver::getMaterialByIndex(int index) {
	for (Material material : materials) {
		if (material.getIndex() == index) {
			return material;
		}
	}
	throw Error("Material with the specified index does not exist\n");
}

ElemParams Solver::createElemParams(std::map<std::string, std::optional<double>> elem_data, double geometry) {
	ElemParams::checkParameters(elem_data);

	int index1 = static_cast<int>(elem_data["index1"].value());
	int index2 = static_cast<int>(elem_data["index2"].value());
	int material_index = static_cast<int>(elem_data["material_index"].value());

	ElemParams elem_params = {
		std::vector<Node> {getNodeByIndex(index1), getNodeByIndex(index2)},
		getMaterialByIndex(material_index),
		std::vector<int> {index1, index2},
		geometry
	};

	return elem_params;
}

int Solver::calculateMatrixSize() {
	int all_dofs = 2 * nodes.size();
	return all_dofs;
}

ublas::matrix<double> Solver::assembleMatrices(ublas::matrix<double>& Klocal, ublas::matrix<double>& Kglobal, IElement* elem
) {
	std::vector<Node> nodes = elem->getNodes();
	
	std::vector<int> indexes;

	// �������������� -1 ��� ��� ���������� � �������� � ����
	for (Node node : nodes) {
		indexes.push_back(node.getIndex() * 2 - 1 - 1);
		indexes.push_back(node.getIndex() * 2 - 1);
	}

	for (int i = 0; i < indexes.size(); i++) {
		for (int j = 0; j < indexes.size(); j++) {
			Kglobal(indexes[i], indexes[j]) += Klocal(i, j);
		}
	}

	return Kglobal;
};

ublas::matrix<double> Solver::createKGlobal(int matrix_size) {

	ublas::zero_matrix<double> zero_matrix(matrix_size, matrix_size);
	ublas::matrix<double> Kglobal = zero_matrix;

	for (auto elem : elements) {
		ublas::matrix<double> k_matrix_elem = elem->KMatrixElemGlobal();

		Kglobal = assembleMatrices(k_matrix_elem, Kglobal, elem);
	}

	Kglobal = applySupports(Kglobal, matrix_size);

	return Kglobal;
}

ublas::vector<double> Solver::createFGlobal(int vector_size) {
	ublas::zero_vector<double> zero_vector(vector_size);
	ublas::vector<double> Fglobal = zero_vector;

	int index;

	for (auto force : forces) {
		index = force.getIndex() - 1; //���������� � ������������ � ����

		Fglobal(2 * index) += force.getForceX(); 
		Fglobal(2 * index + 1) += force.getForceY();
	}
	
	return Fglobal;
}

ublas::matrix<double> Solver::applySupports(ublas::matrix<double>& Kglobal, int matrix_size) {

	ublas::zero_vector<double> zero_vector(matrix_size);

	for (Support support : supports) { 

		std::vector<int> indexes = getDofIndexes(support);

		for (int index : indexes) {
			ublas::column(Kglobal, index) = zero_vector;
			ublas::row(Kglobal, index) = zero_vector;
			Kglobal(index, index) = 1;
		}
	}

	return Kglobal;
}

std::vector<int> Solver::getDofIndexes(Support support) {
	std::vector<int> indexes;

	int node_support_index = support.getIndex() - 1; // ���������� � ������������ � �������

	if (support.getDispX().has_value()) {
		indexes.push_back(2 * node_support_index);
	}
	if (support.getDispY().has_value()) {
		indexes.push_back(2 * node_support_index + 1);
	}

	return indexes;
}


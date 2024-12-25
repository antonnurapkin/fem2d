#include <fstream>
#include <string>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "Solver.h"
#include "utils/tools.h"


Solver::Solver(Preprocessor& preprocessor): preprocessor(preprocessor) {}

void Solver::run() {

	int matrix_size = calculateMatrixSize();

	ublas::matrix<double> Kglobal = createKGlobal(matrix_size);

	ublas::vector<double> Fvector = createFGlobal(matrix_size);

	ublas::vector<double> dispSolution = solveSystem(Kglobal, Fvector);

	setSolutionToNodes(dispSolution);

	std::cout << "Solution:\n";
	std::cout << dispSolution;

	for (Node* node: preprocessor.getNodes()) {
		std::cout << "x: " <<node->getDispX() << " y: " << node->getDispY() << std::endl;
	}
}


int Solver::calculateMatrixSize() {
	int all_dofs = 2 * preprocessor.getNodes().size();
	return all_dofs;
}

ublas::matrix<double> Solver::assembleMatrices(ublas::matrix<double>& Klocal, ublas::matrix<double>& Kglobal, IElement* elem
) {
	std::vector<Node*> nodes = elem->getNodes();
	
	std::vector<int> indexes;

	// �������������� -1 ��� ��� ���������� � �������� � ����
	for (Node* node : nodes) {
		indexes.push_back(node->getIndex() * 2 - 1 - 1);
		indexes.push_back(node->getIndex() * 2 - 1);
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

	for (auto elem : preprocessor.getElements()) {
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

	for (auto force : preprocessor.getForces()) {
		index = force.getIndex() - 1; //���������� � ������������ � ����

		Fglobal(2 * index) += force.getForceX(); 
		Fglobal(2 * index + 1) += force.getForceY();
	}
	
	return Fglobal;
}

ublas::matrix<double> Solver::applySupports(ublas::matrix<double>& Kglobal, int matrix_size) {

	ublas::zero_vector<double> zero_vector(matrix_size);

	for (Support support : preprocessor.getSupports()) { 

		std::vector<int> indexes = preprocessor.getDofIndexes(support);

		for (int index : indexes) {
			ublas::column(Kglobal, index) = zero_vector;
			ublas::row(Kglobal, index) = zero_vector;
			Kglobal(index, index) = 1;
		}
	}

	return Kglobal;
}

void Solver::setSolutionToNodes(ublas::vector<double> solution) {
	
	for (int i = 0; i < preprocessor.getNodes().size(); i++) {
		preprocessor.getNodes()[i]->setDispX(solution[2 * i]);
		preprocessor.getNodes()[i]->setDispY(solution[2 * i + 1]);
	}
}

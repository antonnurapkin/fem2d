#pragma once
#include<map>
#include <vector>
#include <array>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "../preprocessor/elem_service/IElement.h"
#include "../preprocessor/preprocessor.h"



class Solver
{
private:
	Preprocessor preprocessor;
public:
	Solver(Preprocessor& preprocessor);
	void run();

	ublas::matrix<double> createKGlobal(int matrix_size);
	ublas::vector<double> createFGlobal(int vector_size);

	ublas::matrix<double> assembleMatrices(ublas::matrix<double>& Klocal, ublas::matrix<double>& Kglobal, IElement* elem);
	ublas::matrix<double> applySupports(ublas::matrix<double>& Kglobal, int matrix_size);
	
	int calculateMatrixSize();

	void setSolutionToNodes(ublas::vector<double> solution);
};


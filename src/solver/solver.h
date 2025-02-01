#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

// Прочие объявления классов
class Preprocessor;
class IElement;

class Solver
{
private:
    Preprocessor& preprocessor;
    boost::numeric::ublas::vector<double> dispSolution; // Убедись, что ublas правильно указан
public:
    Solver(Preprocessor& preprocessor);
    void run();

    boost::numeric::ublas::matrix<double> createKGlobal(int matrix_size);
    boost::numeric::ublas::vector<double> createFGlobal(int vector_size);

    boost::numeric::ublas::matrix<double> assembleMatrices(boost::numeric::ublas::matrix<double>& Klocal, boost::numeric::ublas::matrix<double>& Kglobal, std::shared_ptr<IElement>);
    boost::numeric::ublas::matrix<double> applySupports(boost::numeric::ublas::matrix<double>& Kglobal, int matrix_size);
    
    int calculateMatrixSize();

    void setSolutionToNodes(boost::numeric::ublas::vector<double> solution);

    boost::numeric::ublas::vector<double> getSolution();
};

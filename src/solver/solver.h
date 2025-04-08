#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

class Preprocessor;
class IElement;

class Solver
{
public:
    Solver(Preprocessor& preprocessor);
    void run();

    boost::numeric::ublas::matrix<double> createKGlobal(int matrix_size) const ;
    boost::numeric::ublas::vector<double> createFGlobal(int vector_size) const ;

    boost::numeric::ublas::matrix<double> assembleMatrices(boost::numeric::ublas::matrix<double>& Klocal, boost::numeric::ublas::matrix<double>& Kglobal, std::shared_ptr<IElement>) const;
    boost::numeric::ublas::matrix<double> applySupports(boost::numeric::ublas::matrix<double>& Kglobal, int matrix_size) const;
    
    int calculateMatrixSize() const;

    void setSolutionToNodes(boost::numeric::ublas::vector<double>& solution);

    boost::numeric::ublas::vector<double> getSolution() const;
private:
    Preprocessor& preprocessor;
    boost::numeric::ublas::vector<double> dispSolution;
};

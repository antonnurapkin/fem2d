#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "preprocessor.h"
#include "elem_service/ielement.h"

class Solver final
{
public:
    Solver(Preprocessor& preprocessor);
    ~Solver();
    Solver(const Solver& other) = delete;
    Solver(Solver&& other) = delete;
    Solver& operator=(const Solver& other);
    Solver& operator=(Solver&& other) = delete;

    void run();

    boost::numeric::ublas::matrix<double> createKGlobal(int matrix_size) const ;
    boost::numeric::ublas::vector<double> createFGlobal(int vector_size) const ;

    boost::numeric::ublas::matrix<double> assembleMatrices(boost::numeric::ublas::matrix<double>& Klocal, boost::numeric::ublas::matrix<double>& Kglobal, std::shared_ptr<IElement>) const;
    boost::numeric::ublas::matrix<double> applySupports(boost::numeric::ublas::matrix<double>& Kglobal, int matrix_size) const;
    
    int calculateMatrixSize() const;

    void setSolutionToNodes(boost::numeric::ublas::vector<double>& solution);

    boost::numeric::ublas::vector<double> getSolution() const;
    Preprocessor& getPreprocessor();
private:
    Preprocessor& preprocessor_;
    boost::numeric::ublas::vector<double> dispSolution_;
};

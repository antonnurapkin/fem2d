#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/assignment.hpp>

namespace solver_math {
    using namespace boost::numeric;

    double getDet(ublas::matrix<double> matrix_);

    ublas::vector<double> solveSystem(ublas::matrix<double>& A_matrix, ublas::vector<double>& b_vector);

    ublas::vector<double> backwardGaussToUp(ublas::matrix<double>& A_matrix, ublas::vector<double>& b_vector);

    ublas::vector<double> backwardGaussToDown(ublas::matrix<double>& A_traingle_lower, ublas::vector<double>& b_vector);

    ublas::matrix<double> Kholetsky_decomposition(ublas::matrix<double>& A);

    void print_matrix(ublas::matrix<double>& A_matrix);
} // namespace solver_math 



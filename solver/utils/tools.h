#pragma once
#include <string>
#include <map>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/assignment.hpp>


using namespace boost::numeric;

double getDet(ublas::matrix<double> matrix_);

ublas::vector<double> solveSystem(ublas::matrix<double>& A, ublas::vector<double>& b);

void print_matrix(ublas::matrix<double>& A);

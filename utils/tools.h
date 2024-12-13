#pragma once
#include <string>
#include <map>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/assignment.hpp>


using namespace boost::numeric;

std::string getElementType(std::string line);

double getSection(std::string line);

std::map<std::string, std::optional<double>> getDataFromString(std::string line, std::vector<std::string> keys);

ublas::matrix<double> TransformMatrix(double angle);

double getDet(ublas::matrix<double> matrix_);

ublas::vector<double> solveSystem(ublas::matrix<double>& A, ublas::vector<double>& b);

void print_matrix(ublas::matrix<double>& A);

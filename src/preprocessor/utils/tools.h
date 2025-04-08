#pragma once
#include <string>
#include <map>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric;

std::string getElementType(std::string line);

double getSection(std::string line);

std::unordered_map<std::string, std::optional<double>> getDataFromString(std::string line, std::vector<std::string> keys);

ublas::matrix<double> TransformMatrix(double angle);
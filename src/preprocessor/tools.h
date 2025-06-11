#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace preprocessor_tools {
using namespace boost::numeric;

std::string getElementType(std::string line);

double getSection(std::string line);

std::unordered_map<std::string, std::optional<double>> getDataFromString(std::string line, std::vector<std::string> keys);
}  // namespace preprocessor_tools

namespace preprocessor_math {
using namespace boost::numeric;

ublas::matrix<double> TransformMatrix(double angle);
}  // namespace preprocessor_math
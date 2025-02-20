#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/assignment.hpp>
#include "Error.h"
#include "tools.h"

using namespace boost::numeric;


std::string getElementType(std::string line) {
    size_t index = line.find(",");
    if (index != std::string::npos) {
        std::string etype = line.substr(index + 1);
        return etype;
    }
    else {
        std::cerr << "Invalid element type setting\n";
        std::exit(1);
    }
}

std::map<std::string, std::optional<double>> getDataFromString(std::string line, std::vector<std::string> keys) {
    std::map<std::string, std::optional<double>> result;

    for (const auto& key : keys) {
        result[key] = std::nullopt; // Init all values as std::nullopt
    }
    
    size_t start_index = line.find(",");

    if (start_index != std::string::npos) {
        line = line.substr(start_index + 1);
        std::stringstream ss(line);

        int index = 0;

        std::string temp_string;
        while (getline(ss, temp_string, ',')) {
            if (index < keys.size()) { // Make sure index is within the bounds of keys
                if (temp_string.empty()) {
                result[keys[index]] = std::nullopt;
                }
                else {
                    result[keys[index]] = std::stod(temp_string);
                }
            index++;
            }
        }

        return result;
    }
    else {
        throw PreprocessorError("Invalid configuration file");
        return result;
    }
}

double getSection(std::string line) {
    size_t index = line.find(",");
    if (index != std::string::npos) {
        std::string section = line.substr(index + 1);
        return std::stod(section);
    }
    else {
        std::cerr << "Invalid element type setting\n";
    }
}

ublas::matrix<double> TransformMatrix(double angle) {
    ublas::matrix<double> T_matrix(4, 4);

    T_matrix <<= cos(angle), sin(angle), 0, 0,
        -sin(angle), cos(angle), 0, 0,
        0, 0, cos(angle), sin(angle),
        0, 0, -sin(angle), cos(angle);

    return T_matrix;
}


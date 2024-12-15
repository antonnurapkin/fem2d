#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
#include <boost/numeric/ublas/io.hpp>
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
    size_t start_index = line.find(",");

    if (start_index != std::string::npos) {
        line = line.substr(start_index + 1);
        std::stringstream ss(line);

        int index = 0;

        std::string temp_string;
        while (getline(ss, temp_string, ','))
        {
            if (temp_string.empty()) {
                result[keys[index]] = std::nullopt;
            }
            else {
                result[keys[index]] = std::stod(temp_string);
            }
            index++;
        }

        return result;
    }
    else {
        throw Error("Invalid configuration file");
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

double getDet(ublas::matrix<double> matr) {
    ublas::permutation_matrix<std::size_t> pm(matr.size1());
    ublas::lu_factorize(matr, pm);

    // ¬ычисл€ем детерминант
    double det = 1.0;
    for (std::size_t i = 0; i < matr.size1(); ++i) {
        det *= matr(i, i);
    }

    // ѕримен€ем перестановку дл€ учета знака детерминанта
    int sign = 1;
    for (std::size_t i = 0; i < matr.size1(); ++i) {
        if (pm(i) != i) {
            sign *= -1;
        }
    }
    det *= sign;

    return det;
}

ublas::vector<double> solveSystem(ublas::matrix<double>& A, ublas::vector<double>& b) {
    ublas::vector<double> solution(b.size());

    double detA = getDet(A);
    
    ublas::matrix<double> B;
    double detB;

    for (int i = 0; i < solution.size(); i++) {
        B = A;
        ublas::column(B, i) = b;

        detB = getDet(B);
        solution(i) = detB / detA;
    }

    return solution;
}

void print_matrix(ublas::matrix<double>& A) {
    for (int i = 0; i < A.size1(); i++) {
        for (int j = 0; j < A.size2(); j++) {
            std::cout << A(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
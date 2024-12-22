#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <optional>
#include <boost/numeric/ublas/io.hpp>
#include "tools.h"

using namespace boost::numeric;

double getDet(ublas::matrix<double> matr) {
    ublas::permutation_matrix<std::size_t> pm(matr.size1());
    ublas::lu_factorize(matr, pm);

    // ��������� �����������
    double det = 1.0;
    for (std::size_t i = 0; i < matr.size1(); ++i) {
        det *= matr(i, i);
    }

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
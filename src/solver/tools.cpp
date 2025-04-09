#include <boost/numeric/ublas/io.hpp>
#include "tools.h"

using namespace boost::numeric;

double solver_math::getDet(ublas::matrix<double> matr) {
    ublas::permutation_matrix<std::size_t> pm(matr.size1());
    ublas::lu_factorize(matr, pm);

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

ublas::matrix<double> solver_math::Kholetsky_decomposition(ublas::matrix<double>& A) {

    ublas::matrix<double> U_lower(A.size1(), A.size1(), 0);

    for (int i = 0; i < A.size1(); i++) {
        for(int j = 0; j <= i; j++) {
            double sum = 0;

            for (int k = 0; k < j; k++) {
                    sum += U_lower(i, k) * U_lower(j, k);
                }

            if (i == j) {
                U_lower(i, j) = std::sqrt(A(j, j) - sum);
            }
            else {
                U_lower(i, j) = (A(i, j) - sum) / U_lower(j, j);
            }
        }
    }
    return U_lower;
}

ublas::vector<double> solver_math::backwardGaussToDown(ublas::matrix<double>& A_traingle_lower, ublas::vector<double>& b_vector) {
    int size = A_traingle_lower.size1();

    ublas::vector<double> solution(size);
    
    solution(0) = b_vector(0) / A_traingle_lower(0, 0);
    
    for (int i = 1; i < size; i++) {
        double temp = 0;
        
        for (int k = 0; k < i; k++ ) {
            double A_ki = A_traingle_lower(k, i);
            double sol_k = solution(k);
            temp += A_traingle_lower(i, k) * solution(k);
        }
        
        solution(i) = (b_vector(i) - temp) / A_traingle_lower(i, i);
    }

    return solution;
}

ublas::vector<double> solver_math::backwardGaussToUp(ublas::matrix<double>& A_matrix, ublas::vector<double>& b_vector) {
    int size = A_matrix.size1();

    ublas::vector<double> solution(size);
    
    solution(size - 1) = b_vector(size - 1) / A_matrix(size - 1, size - 1);
    
    for (int k = size - 1; k >= 0; k--) {
        double temp = 0;
        
        for (int j = k + 1; j < size; j++ ) {
            temp += A_matrix(k, j) * solution(j);
        }
        
        solution(k) = (b_vector(k) - temp) / A_matrix(k, k);
    }

    return solution;
}

ublas::vector<double> solver_math::solveSystem(ublas::matrix<double>& A_matrix, ublas::vector<double>& b_vector) {

    ublas::matrix<double> U_lower = Kholetsky_decomposition(A_matrix);

    ublas::vector<double> solution_temp = backwardGaussToDown(U_lower, b_vector);

    ublas::matrix<double> U_upper =  ublas::trans(U_lower);
    
    return backwardGaussToUp(U_upper, solution_temp);
}

void solver_math::print_matrix(ublas::matrix<double>& A_matrix) {
    for (int i = 0; i < A_matrix.size1(); i++) {
        for (int j = 0; j < A_matrix.size2(); j++) {
            std::cout << A_matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
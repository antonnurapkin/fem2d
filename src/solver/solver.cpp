#include "solver.h"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <future>
#include <thread>
#include <utility>
#include <vector>

#include "error.cpp"
#include "preprocessor/elem_service/ielement.h"
#include "preprocessor/node/node.h"
#include "preprocessor/preprocessor.h"
#include "tools.h"

Solver::Solver(Preprocessor& preprocessor) : preprocessor_(preprocessor) {
    std::cout << "Solver was created successfully!\n";
}

Solver::~Solver() {
    std::cout << "Solver was deleted successfully!\n";
}

Solver& Solver::operator=(const Solver& other) {
    if (&other != this) {
        preprocessor_ = other.preprocessor_;
        dispSolution_ = other.dispSolution_;
    }
    return *this;
}

void Solver::run() {
    try {
        int matrix_size = calculateMatrixSize();

        auto fut_k_global = std::async(std::launch::async, [this, matrix_size]() { return this->createKGlobal(matrix_size); });

        auto fut_f_global = std::async(std::launch::async, [this, matrix_size]() { return this->createFGlobal(matrix_size); });

        ublas::matrix<double> Kglobal = fut_k_global.get();
        ublas::vector<double> Fvector = fut_f_global.get();

        dispSolution_ = solver_math::solveSystem(Kglobal, Fvector);

        setSolutionToNodes(dispSolution_);

        std::cout << "The system of equations was solved successfully!\n";
    } catch (const std::exception& err) {
        throw SolverError(err.what());
    }
}

int Solver::calculateMatrixSize() const {
    return 2 * preprocessor_.getNodes().size();
}

ublas::matrix<double> Solver::assembleMatrices(ublas::matrix<double>& Klocal, ublas::matrix<double>& Kglobal, std::shared_ptr<IElement> elem) const {
    std::vector<std::shared_ptr<Node>> nodes = elem->getNodes();

    std::vector<int> indexes;

    // additional minus 1 because indexing of nodes starts from 1
    for (const std::shared_ptr<Node>& node : nodes) {
        indexes.push_back(node->getIndex() * 2 - 1 - 1);
        indexes.push_back(node->getIndex() * 2 - 1);
    }

    for (int i = 0; i < indexes.size(); i++) {
        for (int j = 0; j < indexes.size(); j++) {
            Kglobal(indexes[i], indexes[j]) += Klocal(i, j);
        }
    }

    return Kglobal;
};

ublas::matrix<double> Solver::createKGlobal(int matrix_size) const {
    ublas::zero_matrix<double> zero_matrix(matrix_size, matrix_size);
    ublas::matrix<double> Kglobal = zero_matrix;

    for (const std::shared_ptr<IElement>& elem : preprocessor_.getElements()) {
        ublas::matrix<double> k_matrix_elem = elem->KMatrixElemGlobal();

        Kglobal = assembleMatrices(k_matrix_elem, Kglobal, elem);
    }

    Kglobal = applySupports(Kglobal, matrix_size);

    return Kglobal;
}

ublas::vector<double> Solver::createFGlobal(int vector_size) const {
    ublas::zero_vector<double> zero_vector(vector_size);
    ublas::vector<double> Fglobal = zero_vector;

    int index;

    for (const Force& force : preprocessor_.getForces()) {
        index = force.getIndex() - 1;  // additional minus 1 because indexing of nodes starts from 1

        Fglobal(2 * index) += force.getForceX();
        Fglobal(2 * index + 1) += force.getForceY();
    }

    return Fglobal;
}

ublas::matrix<double> Solver::applySupports(ublas::matrix<double>& Kglobal, int matrix_size) const {
    ublas::zero_vector<double> zero_vector(matrix_size);

    for (const Support& support : preprocessor_.getSupports()) {
        std::vector<int> indexes = preprocessor_.getDofIndexes(support);

        for (int index : indexes) {
            ublas::column(Kglobal, index) = zero_vector;
            ublas::row(Kglobal, index) = zero_vector;
            Kglobal(index, index) = 1;
        }
    }

    return Kglobal;
}

void Solver::setSolutionToNodes(ublas::vector<double>& solution) {
    for (int i = 0; i < preprocessor_.getNodes().size(); i++) {
        preprocessor_.getNodes()[i]->setDispX(solution[2 * i]);
        preprocessor_.getNodes()[i]->setDispY(solution[2 * i + 1]);
    }
}

ublas::vector<double> Solver::getSolution() const {
    return dispSolution_;
}

Preprocessor& Solver::getPreprocessor() {
    return preprocessor_;
}
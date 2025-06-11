#include <iostream>

#include "postprocessor/postprocessor.h"
#include "preprocessor/error.h"
#include "preprocessor/preprocessor.h"
#include "solver/error.h"
#include "solver/solver.h"

int main(int argc, char* argv[]) {
    try {
        Preprocessor preprocessor(argc, argv);
        preprocessor.readConfig();

        Solver solver(preprocessor);
        solver.run();

        Postprocessor postprocessor(solver);
        postprocessor.run();
    } catch (const PreprocessorError& err) {
        std::cout << "PreprocessorError:\n" << err.what();
        return -1;
    } catch (const SolverError& err) {
        std::cout << "SolverError:\n" << err.what();
        return -1;
    } catch (const std::exception& err) {
        std::cout << err.what();
        return -1;
    } catch (...) {
        std::cerr << "Caught an unknown exception!\n";
        return -1;
    }
    std::cout << "The program has finished working!\n";

    return 0;
}
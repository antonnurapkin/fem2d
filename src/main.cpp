#include <iostream>
#include "solver/solver.h"
#include "preprocessor/preprocessor.h"
#include "postprocessor/postprocessor.h"
#include "preprocessor/error.h"


int main(int argc, char* argv[])
{
	try {
		Preprocessor preprocessor(argc, argv);
		preprocessor.readConfig();

		Solver solver(preprocessor);
		solver.run();

		Postprocessor postprocessor(preprocessor, solver);
		postprocessor.run();
	} catch (const PreprocessorError& err) {
		std::cout << "PreprocessorError:\n" <<err.what();
	} catch (const std::exception& err) {
		std::cout << err.what();
	} catch (...) {
		std::cerr << "Caught an unknown exception!\n";
	}
	std::cout << "The program has finished working\n";

    return 0;
}
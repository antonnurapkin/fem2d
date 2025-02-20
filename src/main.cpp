#include <iostream>
#include <string>

#include "solver/solver.h"
#include "preprocessor/preprocessor.h"
#include "postprocessor/postprocessor.h"


int main(int argc, char* argv[])
{
	Preprocessor preprocessor(argc, argv);

	preprocessor.readConfig();

	Solver solver(preprocessor);

	solver.run();

	Postprocessor postprocessor(preprocessor, solver);

	postprocessor.run();

    return 0;
}
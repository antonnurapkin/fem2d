#include <iostream>
#include <string>

#include "solver/Solver.h"
#include "postprocessor/postprocessor.h"

int main(int argc, char* argv[])
{
	Preprocessor preprocessor(argc, argv);

	preprocessor.readConfig();

	Solver solver(preprocessor);

	solver.run();

	Postprocessor postprocessor(preprocessor);

	postprocessor.run();

    return 0;
}
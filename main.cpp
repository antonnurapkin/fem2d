#include <iostream>
#include <string>

#include "solver/Solver.h"

int main(int argc, char* argv[])
{
	Preprocessor preprocessor(argc, argv);

	preprocessor.readConfig();

	Solver solver(preprocessor);

	solver.run();

    return 0;
}
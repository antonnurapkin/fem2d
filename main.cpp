#include <iostream>
#include <string>

#include "solver/Solver.h"
#include "postprocessor/postprocessor.h"

// TODO: добавить обработку случая в конфиге:
// DISP,1,0,0
// DISP,7,0,

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
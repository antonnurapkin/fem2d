#include <iostream>
#include <string>

#include "solver/Solver.h"

int main()
{
	std::string path_to_config_file;
	std::cout << "Enter path to config file:\n";

	std::cin >> path_to_config_file;

	Preprocessor preprocessor(path_to_config_file);

	preprocessor.readConfig();

	Solver solver(preprocessor);

	solver.run();

    return 0;
}
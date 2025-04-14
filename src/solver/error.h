#pragma once
#include <exception>
#include <string>

class SolverError: public std::exception
{
private:
	std::string message;
public:
    SolverError(const char* msg);
	const char* what() const throw();
};
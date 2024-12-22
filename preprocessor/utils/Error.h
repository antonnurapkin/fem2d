#pragma once
#include <exception>
#include <string>

class Error: public std::exception
{
private:
	std::string message;
public:
	Error(const char* msg);
	const char* what() const throw();
};


#include "Error.h"

Error::Error(const char* msg) : message(msg) {};

const char* Error::what() const throw() {
	return message.c_str();
}


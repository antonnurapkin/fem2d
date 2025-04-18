#include "error.h"

SolverError::SolverError(const char* msg) : message(msg){};

const char* SolverError::what() const throw() {
    return message.c_str();
}

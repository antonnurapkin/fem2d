#include "error.h"

PostprocessorError::PostprocessorError(const char* msg) : message(msg){};

const char* PostprocessorError::what() const throw() {
    return message.c_str();
}

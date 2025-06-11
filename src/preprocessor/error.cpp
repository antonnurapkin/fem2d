#include "error.h"

PreprocessorError::PreprocessorError(const char* msg) : message(msg){};

const char* PreprocessorError::what() const throw() {
    return message.c_str();
}

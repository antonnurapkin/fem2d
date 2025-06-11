#pragma once
#include <exception>
#include <string>

class PreprocessorError : public std::exception {
   private:
    std::string message;

   public:
    PreprocessorError(const char* msg);
    const char* what() const throw();
};

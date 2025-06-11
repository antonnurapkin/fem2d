#pragma once
#include <exception>
#include <string>

class PostprocessorError : public std::exception {
   private:
    std::string message;

   public:
   PostprocessorError(const char* msg);
    const char* what() const throw();
};
#include "../preprocessor/preprocessor.h"

class Postprocessor{
private:
    Preprocessor preprocessor;
public:
    Postprocessor(Preprocessor& preprocessor);

    void run();


};
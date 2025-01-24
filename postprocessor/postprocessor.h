#pragma once
#include "renderer/rendererManager.h"

class Preprocessor;
class Solver;

class Postprocessor {
private:
    Preprocessor& preprocessor;
    Solver& solver;

    RendererManager rendererManager;

public:
    Postprocessor(Preprocessor& preprocessor, Solver& solver);

    void run();
};
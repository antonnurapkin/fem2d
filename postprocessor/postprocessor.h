#pragma once

#include "renderer/rendererManager.h"
#include "../solver/Solver.h"


class Postprocessor {
private:
    Preprocessor& preprocessor;
    Solver& solver;

    RendererManager rendererManager;

public:
    Postprocessor(Preprocessor& preprocessor, Solver& solver);

    void run();
};
#pragma once
#include "renderer/rendererManager.h"
#include "solver/solver.h"
#include "preprocessor/preprocessor.h"

class Postprocessor {
public:
    Postprocessor(Solver& solver);

    void run();
private:
    Solver& solver;

    RendererManager rendererManager;
};
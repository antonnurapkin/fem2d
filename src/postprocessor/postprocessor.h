#pragma once
#include "renderer/rendererManager.h"
#include "solver/solver.h"
#include "preprocessor/preprocessor.h"

class Postprocessor {
private:
    Solver& solver;

    RendererManager rendererManager;

public:
    Postprocessor(Solver& solver);

    void run();
};
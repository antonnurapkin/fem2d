#pragma once
#include "renderer/renderer_manager.h"
#include "solver/solver.h"
#include "preprocessor/preprocessor.h"

class Postprocessor {
public:
    Postprocessor(Solver& solver);
    ~Postprocessor();

    void run();
private:
    Solver& solver;

    RendererManager rendererManager;
};
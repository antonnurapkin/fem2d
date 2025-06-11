#pragma once
#include "preprocessor/preprocessor.h"
#include "renderer/renderer_manager.h"
#include "solver/solver.h"

class Postprocessor {
   public:
    Postprocessor(Solver& solver);
    ~Postprocessor();

    void run();

   private:
    Solver& solver_;
    RendererManager rendererManager_;
};
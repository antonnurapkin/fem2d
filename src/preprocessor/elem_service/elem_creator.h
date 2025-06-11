#pragma once
#include <string>

#include "elem_params.h"
#include "ielement.h"

class ElemCreator {
   public:
    static std::shared_ptr<IElement> createElement(const std::string& type, ElemParams& elemParams);
};
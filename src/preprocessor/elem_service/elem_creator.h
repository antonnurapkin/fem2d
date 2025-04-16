#pragma once
#include <string>
#include "ielement.h"
#include "elem_params.h"

class ElemCreator
{
public:
	static std::shared_ptr<IElement> createElement(const std::string& type, ElemParams& elemParams);
};
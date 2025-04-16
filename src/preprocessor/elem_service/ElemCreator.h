#pragma once
#include <string>
#include "IElement.h"
#include "ElemParams.h"

class ElemCreator
{
public:
	static std::shared_ptr<IElement> createElement(const std::string& type, ElemParams& elemParams);
};
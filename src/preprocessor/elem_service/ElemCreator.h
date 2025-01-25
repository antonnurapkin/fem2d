#pragma once
#include <string>
#include "IElement.h"
#include "ElemParams.h"

class ElemCreator
{
public:
	static IElement* getElement(std::string type, ElemParams& elemParams);
};


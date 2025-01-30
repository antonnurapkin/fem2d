#pragma once
#include <string>

class IElement;
class ElemParams;

class ElemCreator
{
public:
	static IElement* getElement(std::string type, ElemParams& elemParams);
};


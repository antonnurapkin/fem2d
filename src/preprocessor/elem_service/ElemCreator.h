#pragma once
#include <string>

class IElement;
class ElemParams;

class ElemCreator
{
public:
	static std::shared_ptr<IElement> getElement(std::string type, ElemParams& elemParams);
};


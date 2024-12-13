#include "ElemCreator.h"
#include "Truss.h"
#include "ElemParams.h"

IElement* ElemCreator::getElement(std::string type, ElemParams& elemParams)
{
    if (type == "TRUSS") {
        return new Truss(elemParams);
    }
}

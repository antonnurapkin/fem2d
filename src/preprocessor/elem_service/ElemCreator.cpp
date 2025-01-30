#include "elements/Truss.h"
#include "ElemParams.h"
#include "IElement.h"
#include "ElemCreator.h"

IElement* ElemCreator::getElement(std::string type, ElemParams& elemParams)
{
    if (type == "TRUSS") {
        return new Truss(elemParams);
    }
}

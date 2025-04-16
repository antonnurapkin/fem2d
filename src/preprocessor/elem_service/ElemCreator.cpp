#include <memory>
#include "elements/truss.h"
#include "ElemParams.h"
#include "IElement.h"
#include "ElemCreator.h"

std::shared_ptr<IElement> ElemCreator::createElement(const std::string& type, ElemParams& elemParams)
{
    if (type == "TRUSS") {
        return std::make_shared<Truss>(elemParams);
    }
}

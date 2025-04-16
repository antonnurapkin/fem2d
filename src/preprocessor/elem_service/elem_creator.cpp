#include <memory>
#include "elements/truss.h"
#include "elem_params.h"
#include "ielement.h"
#include "elem_creator.h"

std::shared_ptr<IElement> ElemCreator::createElement(const std::string& type, ElemParams& elemParams)
{
    if (type == "TRUSS") {
        return std::make_shared<Truss>(elemParams);
    }
}

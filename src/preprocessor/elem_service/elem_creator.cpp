#include "elem_creator.h"

#include <memory>

#include "elem_params.h"
#include "elements/truss.h"
#include "ielement.h"

std::shared_ptr<IElement> ElemCreator::createElement(const std::string& type, ElemParams& elemParams) {
    if (type == "TRUSS") {
        return std::make_shared<Truss>(elemParams);
    }
}

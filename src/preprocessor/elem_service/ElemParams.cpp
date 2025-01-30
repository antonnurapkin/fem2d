#include <vector>
#include <string>
#include <optional>
#include <map>
#include "ElemParams.h"
#include "../utils/Error.h"


void ElemParams::checkParameters(std::map<std::string, std::optional<double>> elem_data) {
	if (!elem_data["index1"].has_value() || !elem_data["index2"].has_value() || !elem_data["material_index"].has_value()) {
		throw PreprocessorError("Missing required parameter in any element definition");
	}
}

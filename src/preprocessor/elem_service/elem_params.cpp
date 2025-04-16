#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include "elem_params.h"
#include "error.h"


void ElemParams::checkParameters(std::unordered_map<std::string, std::optional<double>> elem_data) {
	if (!elem_data["index1"].has_value() || !elem_data["index2"].has_value() || !elem_data["material_index"].has_value()) {
		throw PreprocessorError("Missing required parameter in any element definition");
	}
}

ElemParams::ElemParams(std::vector<std::shared_ptr<Node>> nodes, std::vector<int> indexes, Material material, double geom_parameter) :
nodes(nodes), indexes(indexes), material(material), geom_parameter(geom_parameter) {};

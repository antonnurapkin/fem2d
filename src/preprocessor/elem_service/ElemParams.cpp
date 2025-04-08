#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include "ElemParams.h"
#include "../utils/Error.h"
#include "../preprocessor.h"


void ElemParams::checkParameters(std::unordered_map<std::string, std::optional<double>> elem_data) {
	if (!elem_data["index1"].has_value() || !elem_data["index2"].has_value() || !elem_data["material_index"].has_value()) {
		throw PreprocessorError("Missing required parameter in any element definition");
	}
}

ElemParams ElemParams::createElemParams(
	std::unordered_map<std::string, std::optional<double>> elem_data, 
	double geometry, 
	Preprocessor& preprocessor
) {
	ElemParams::checkParameters(elem_data);

	int index1 = static_cast<int>(elem_data.at("index1").value());
	int index2 = static_cast<int>(elem_data.at("index2").value());
	int material_index = static_cast<int>(elem_data.at("material_index").value());

	std::shared_ptr<Node> node1 = preprocessor.getNodeByIndex(index1);
	std::shared_ptr<Node> node2 = preprocessor.getNodeByIndex(index2);
	Material material = preprocessor.getMaterialByIndex(material_index);

	std::vector<std::shared_ptr<Node>> elem_nodes = { node1, node2 };

	ElemParams elem_params = {
		elem_nodes,
		material,
		std::vector<int> {index1, index2},
		geometry
	};

	return elem_params;
}

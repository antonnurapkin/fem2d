#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <optional>
#include "../material/Material.h"
#include "../node/Node.h"

class Preprocessor;

struct ElemParams {
	std::vector<std::shared_ptr<Node>> nodes;
	Material material;
	std::vector<int> indexes;
	double geom_parameter;

	static void checkParameters(std::map<std::string, std::optional<double>> elem_data);

	static ElemParams createElemParams(
		std::map<std::string, std::optional<double>> elem_data,
		double geometry, 
		Preprocessor& preprocessor
	);
};
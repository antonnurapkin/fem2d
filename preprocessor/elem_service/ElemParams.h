#pragma once
#include <vector>
#include <string>
#include <map>
#include "../node/Node.h"
#include "../material/Material.h"

struct ElemParams {
	std::vector<Node> nodes;
	Material material;
	std::vector<int> indexes;
	double geom_parameter;

	static void checkParameters(std::map<std::string, std::optional<double>> elem_data);
};
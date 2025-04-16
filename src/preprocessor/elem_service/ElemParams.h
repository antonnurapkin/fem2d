#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include "material/material.h"
#include "node/node.h"

class Preprocessor;

struct ElemParams {
	std::vector<std::shared_ptr<Node>> nodes;
	Material material;
	std::vector<int> indexes;
	double geom_parameter;

	static void checkParameters(std::unordered_map<std::string, std::optional<double>> elem_data);

	static ElemParams createElemParams(
		std::unordered_map<std::string, std::optional<double>> elem_data,
		double geometry, 
		Preprocessor& preprocessor
	);
};
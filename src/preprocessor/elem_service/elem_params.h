#pragma once
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "material/material.h"
#include "node/node.h"

using ConfigData = std::unordered_map<std::string, std::optional<double>>;

class ElemParams {
   public:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<int> indexes;
    Material material;
    double geom_parameter;

    static void checkParameters(ConfigData elem_data);

    ElemParams(std::vector<std::shared_ptr<Node>> nodes, std::vector<int> indexes, Material material, double geom_parameter);
};
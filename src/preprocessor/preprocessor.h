#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <optional>
#include "material/material.h"
#include "node/node.h"
#include "boundaries/Force.h"
#include "boundaries/Support.h"
#include "elem_service/elem_params.h"
#include "elem_service/ielement.h"


class Preprocessor final{
public:
    Preprocessor(int number_params, char** params);

	std::string getPathToConfig(int number_params, char** params) const;
    void readConfig();

	std::vector<std::shared_ptr<IElement>> getElements() const;
	std::vector<std::shared_ptr<Node>> getNodes() const;
	std::vector<Material> getMaterials() const;
	std::vector<Force> getForces() const;
	std::vector<Support> getSupports() const;

	std::vector<int> getDofIndexes(const Support& support) const;
	std::shared_ptr<Node> getNodeByIndex(int index) const;
	Material getMaterialByIndex(int index) const;
	
private:
	std::string path_to_input_file_;

    std::vector<std::shared_ptr<IElement>> elements_;
	std::vector<std::shared_ptr<Node>> nodes_;
	std::vector<Material> materials_;
	std::vector<Force> forces_;
	std::vector<Support> supports_;

	static const int PARAMETERS_NUMBER = 3;
};
#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <optional>
#include "material/Material.h"
#include "boundaries/Force.h"
#include "boundaries/Support.h"
#include "elem_service/ElemParams.h"

class Node;
class IElement;

class Preprocessor{
private:
	std::string path_to_input_file;

    std::vector<std::shared_ptr<IElement>> elements;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<Material> materials;
	std::vector<Force> forces;
	std::vector<Support> supports;
public:
    Preprocessor(int number_params, char** params);

	std::string getPathToConfig(int number_params, char** params);
    void readConfig();

	std::vector<std::shared_ptr<IElement>> getElements();
	std::vector<std::shared_ptr<Node>> getNodes();
	std::vector<Material> getMaterials();
	std::vector<Force> getForces();
	std::vector<Support> getSupports();

	std::vector<int> getDofIndexes(Support support);

	std::shared_ptr<Node> getNodeByIndex(int index);
	Material getMaterialByIndex(int index);
	
};
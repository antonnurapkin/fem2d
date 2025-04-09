#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <optional>
#include "elem_service/IElement.h"
#include "node/node.h"
#include "material/material.h"
#include "boundaries/Force.h"
#include "boundaries/Support.h"
#include "elem_service/ElemParams.h"
#include "elem_service/ElemCreator.h"
#include "tools.h"
#include "error.h"
#include "preprocessor.h"

using ConfigData = std::unordered_map<std::string, std::optional<double>>;

Preprocessor::Preprocessor(int number_params, char** params): path_to_input_file_(getPathToConfig(number_params, params)) {
	std::cout << "Preprocessor was created successfully!\n";
}

std::string Preprocessor::getPathToConfig(int number_params, char** params) const {
	if (number_params < PARAMETERS_NUMBER) {
		throw PreprocessorError("Not enough launch arguments!");
	} 
	else if (std::strcmp(params[1], "--input") != 0) {\
		throw PreprocessorError("Unknown launch argument");	
	} 
	else {
		return std::string(params[2]);
	}
}

void Preprocessor::readConfig() {
	try {
		std::fstream file(path_to_input_file_);
		if (file.is_open()) {
			std::string line;
			std::string etype;
			double section;

			while (std::getline(file, line)) {

				if (line.find("ETYPE") != std::string::npos && etype.size() == 0) {
					etype = preprocessor_tools::getElementType(line);
				}

				else if (line.find("MATERIAL") != std::string::npos) {
					ConfigData material_data = preprocessor_tools::getDataFromString(line, { "Emod", "mu", "density", "index" });
					Material material = Material::createMaterial(material_data);
					materials_.push_back(material);
				}

				else if (line.find("SECTION") != std::string::npos) {
					section = preprocessor_tools::getSection(line);
				}

				else if (line.find("NODE") != std::string::npos) {
					ConfigData node_data = preprocessor_tools::getDataFromString(line, { "index", "x", "y" });
					std::shared_ptr<Node> node = Node::createNode(node_data);
					nodes_.push_back(node);
				}
				
				else if (line.find("ELEM") != std::string::npos) {
					ConfigData elem_data = preprocessor_tools::getDataFromString(line, { "index1", "index2", "material_index" });

					ElemParams elem_params = ElemParams::createElemParams(elem_data, section, *this);
					std::shared_ptr<IElement> elem = ElemCreator::createElement(etype, elem_params);
					elements_.push_back(elem);
				}

				else if (line.find("FORCE") != std::string::npos) {
					ConfigData force_data = preprocessor_tools::getDataFromString(line, { "index", "Fx", "Fy" });
					Force force = Force::createForce(force_data);
					forces_.push_back(force);
				}

				else if (line.find("DISP") != std::string::npos) {
					ConfigData support_data = preprocessor_tools::getDataFromString(line, { "index", "disp_x", "disp_y" });
					Support support = Support::createSupport(support_data);
					supports_.push_back(support);
				}
			}

			std::cout << "Configuration file was readed successfully!\n";
			std::fstream file(path_to_input_file_);
		}
		else {
			throw PreprocessorError("The config file does not exist in this path\n");
		}
	}
	catch (const PreprocessorError& err) {
		std::cout << "\nError while config file reading:\n" << err.what() << std::endl;
		std::exit(1);
	}
}

std::shared_ptr<Node> Preprocessor::getNodeByIndex(int index) const {
	for (const std::shared_ptr<Node>& node : nodes_) {
		if (node->getIndex() == index) {
			return node;
		}
	}
	throw PreprocessorError("Node with the specified index does not exist\n");
}

Material Preprocessor::getMaterialByIndex(int index) const {
	for (const Material& material : materials_) {
		if (material.getIndex() == index) {
			return material;
		}
	}
	throw PreprocessorError("Material with the specified index does not exist\n");
}

std::vector<int> Preprocessor::getDofIndexes(const Support& support) const {
	std::vector<int> indexes;

	int node_support_index = support.getIndex() - 1; // ���������� � ������������ � �������

	if (support.getDispX().has_value()) {
		indexes.push_back(2 * node_support_index);
	}
	if (support.getDispY().has_value()) {
		indexes.push_back(2 * node_support_index + 1);
	}

	return indexes;
}

std::vector<std::shared_ptr<IElement>> Preprocessor::getElements() const {
	return elements_;
}

std::vector<std::shared_ptr<Node>> Preprocessor::getNodes() const {
	return nodes_;
}

std::vector<Material> Preprocessor::getMaterials() const {
	return materials_;
}

std::vector<Force> Preprocessor::getForces() const {
	return forces_;
}

std::vector<Support> Preprocessor::getSupports() const {
	return supports_;
}
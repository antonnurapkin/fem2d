#include "preprocessor.h"
#include <iostream>


Preprocessor::Preprocessor(int number_params, char** params) {
	path_to_input_file = getPathToConfig(number_params, params);
}

Preprocessor::~Preprocessor() {
    for (auto* elem : elements) {
		if (elem != nullptr) {
			delete elem;
		}
	}
}

std::string Preprocessor::getPathToConfig(int number_params, char** params) {
	if (number_params < 3) {
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
		std::fstream file(path_to_input_file);
		if (file.is_open()) {
			std::string line;
			std::string etype;
			double section;

			while (std::getline(file, line)) {

				if (line.find("ETYPE") != std::string::npos && etype.size() == 0) {
					etype = getElementType(line);
				}

				else if (line.find("MATERIAL") != std::string::npos) {
					std::map<std::string, std::optional<double>> mat_props = getDataFromString(line, { "Emod", "mu", "density", "index" });
					Material material(mat_props["Emod"], mat_props["mu"], mat_props["density"], mat_props["index"]);
					this->materials.push_back(material);
				}

				else if (line.find("SECTION") != std::string::npos) {
					section = getSection(line);
				}

				else if (line.find("NODE") != std::string::npos) {
					std::map<std::string, std::optional<double>> node_data = getDataFromString(line, { "index", "x", "y" });
					Node node(node_data["index"], node_data["x"], node_data["y"]);
					this->nodes.push_back(node);
				}
				// TODO: ������� �������� ������ ���������� � ����������� �� ���� ���������
				else if (line.find("ELEM") != std::string::npos) {
					std::map<std::string, std::optional<double>> elem_data = getDataFromString(line, { "index1", "index2", "material_index" });

					ElemParams elem_params = createElemParams(elem_data, section);

					IElement* elem = ElemCreator::getElement(etype, elem_params);
					elements.push_back(elem);
				}

				else if (line.find("FORCE") != std::string::npos) {
					std::map<std::string, std::optional<double>> force_components = getDataFromString(line, { "index", "Fx", "Fy" });
					Force force(force_components["index"], force_components["Fx"], force_components["Fy"]);
					this->forces.push_back(force);
				}

				else if (line.find("DISP") != std::string::npos) {
					std::map<std::string, std::optional<double>> support_components = getDataFromString(line, { "index", "disp_x", "disp_y" });
					Support support(support_components["index"], support_components["disp_x"], support_components["disp_y"]);
					this->supports.push_back(support);
				}
			}

			std::cout << "Configuration file was readed successfully!\n";
			std::fstream file(path_to_input_file);
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

Node Preprocessor::getNodeByIndex(int index) {
	for (Node node : nodes) {
		if (node.getIndex() == index) {
			return node;
		}
	}
	throw PreprocessorError("Node with the specified index does not exist\n");
}

Material Preprocessor::getMaterialByIndex(int index) {
	for (Material material : materials) {
		if (material.getIndex() == index) {
			return material;
		}
	}
	throw PreprocessorError("Material with the specified index does not exist\n");
}

ElemParams Preprocessor::createElemParams(std::map<std::string, std::optional<double>> elem_data, double geometry) {
	ElemParams::checkParameters(elem_data);

	int index1 = static_cast<int>(elem_data["index1"].value());
	int index2 = static_cast<int>(elem_data["index2"].value());
	int material_index = static_cast<int>(elem_data["material_index"].value());

	ElemParams elem_params = {
		std::vector<Node> {getNodeByIndex(index1), getNodeByIndex(index2)},
		getMaterialByIndex(material_index),
		std::vector<int> {index1, index2},
		geometry
	};

	return elem_params;
}

std::vector<int> Preprocessor::getDofIndexes(Support support) {
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

std::vector<IElement*> Preprocessor::getElements() {
	return this->elements;
}

std::vector<Node> Preprocessor::getNodes() {
	return this->nodes;
}

std::vector<Material> Preprocessor::getMaterials() {
	return this->materials;
}

std::vector<Force> Preprocessor::getForces() {
	return this->forces;
}

std::vector<Support> Preprocessor::getSupports() {
	return this->supports;
}
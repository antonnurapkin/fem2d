#include "Support.h"
#include "../utils/Error.h"
#include <optional>

Support::Support(int index, std::optional<double> disp_x, std::optional<double> disp_y):
	index(index), disp_x(disp_y), disp_y(disp_y) {};


Support Support::createSupport(std::map<std::string, std::optional<double>> support_components) {
	Support::checkParameters(support_components["index"], support_components["disp_x"], support_components["disp_y"]);

	return Support(
		support_components["index"].value(), 
		support_components["disp_x"], 
		support_components["disp_y"]
	);
}

void Support::checkParameters(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y) {
	if (!index.has_value() || (!disp_x.has_value() && !disp_y.has_value())) {
		throw PreprocessorError("Missing required parameter in displacements definition");
	}
}

std::optional<double> Support::getDispX() const {
	return this->disp_x;
}

std::optional<double> Support::getDispY() const {
	return this->disp_y;
}

void Support::setDispX(std::optional<double> disp_x) {
	this->disp_x = disp_x;
}

void Support::setDispY(std::optional<double> disp_y) {
	this->disp_y = disp_y;
}

int Support::getIndex() const {
	return this->index;
}

void Support::setIndex(int index) {
	this->index = index;
}

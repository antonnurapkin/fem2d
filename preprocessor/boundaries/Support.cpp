#include "Support.h"
#include "../utils/Error.h"
#include <optional>

Support::Support(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y) {
	Support::checkParameters(index, disp_x, disp_y);
	this->index = index.value();
	this->disp_x = disp_x;
	this->disp_y = disp_y;
}

void Support::checkParameters(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y) {
	if (!index.has_value() || (!disp_x.has_value() && !disp_y.has_value())) {
		throw Error("Missing required parameter in displacements definition");
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





#include <optional>
#include <unordered_map>
#include "../error.h"
#include "Force.h"

Force::Force(int i, float x, float y) : index(i), force_x(x), force_y(y) {};

Force Force::createForce(std::unordered_map<std::string, std::optional<double>> force_components) {
	Force::checkParameters(force_components["index"], force_components["Fx"], force_components["Fy"]);

	return Force(
		force_components["index"].value(), 
		force_components["Fx"].value(), 
		force_components["Fy"].value()
	);
}

void Force::checkParameters(std::optional<int> i, std::optional<float> x, std::optional<float> y) {
	if (!i.has_value() || !x.has_value() || !y.has_value()) {
		throw PreprocessorError("Missing required parameter in any force definition");
	}
}

int Force::getIndex() const { return this->index; }
float Force::getForceX() const { return this->force_x; }
float Force::getForceY() const { return this->force_y; }

void Force::setIndex(int i) { this->index = i; }
void Force::setForceX(float x) { this->force_x = x; }
void Force::setForceY(float y) { this->force_y = y; }


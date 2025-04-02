#include <optional>
#include "../utils/Error.h"
#include "Force.h"

Force::Force(int i, float x, float y) : index(i), force_x(x), force_y(y) {};

Force Force::createForce(std::optional<int> i, std::optional<float> x, std::optional<float> y) {
	Force::checkParameters(i, x, y);

	return Force(i.value(), x.value(), y.value());
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


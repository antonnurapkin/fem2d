#include <optional>
#include "../utils/Error.h"
#include "Force.h"

Force::Force(std::optional<int> i, std::optional<float> x, std::optional<float> y)
{
	checkParameters(i, x, y);
	this->force_x = x.value();
	this->force_y = y.value();
	this->index = i.value();
}

void Force::checkParameters(std::optional<int> i, std::optional<float> x, std::optional<float> y) {
	if (!i.has_value() || !x.has_value() || !y.has_value()) {
		throw Error("Missing required parameter in any force definition");
	}
}

int Force::getIndex() const { return this->index; }
float Force::getForceX() const { return this->force_x; }
float Force::getForceY() const { return this->force_y; }

void Force::setIndex(int i) { this->index = i; }
void Force::setForceX(float x) { this->force_x = x; }
void Force::setForceY(float y) { this->force_y = y; }


#include "force.h"

#include <optional>
#include <unordered_map>

#include "error.h"

Force::Force(int i, double x, double y) : index(i), force_x(x), force_y(y){};

Force Force::createForce(std::unordered_map<std::string, std::optional<double>> force_components) {
    Force::checkParameters(force_components["index"], force_components["Fx"], force_components["Fy"]);

    return Force(force_components["index"].value(), force_components["Fx"].value(), force_components["Fy"].value());
}

void Force::checkParameters(std::optional<int> i, std::optional<double> x, std::optional<double> y) {
    if (!i.has_value() || !x.has_value() || !y.has_value()) {
        throw PreprocessorError("Missing required parameter in any force definition");
    }
}

int Force::getIndex() const {
    return this->index;
}

double Force::getForceX() const {
    return this->force_x;
}

double Force::getForceY() const {
    return this->force_y;
}

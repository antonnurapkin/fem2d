#include <optional>
#include "Node.h"
#include "../utils/Error.h"


Node::Node(std::optional<int> index, std::optional<double> x, std::optional<double> y) {
	Node::checkParameters(index, x, y);

	this->index = index.value();
	this->x = x.value();
	this->y = y.value();
}

void Node::checkParameters(std::optional<int> index, std::optional<double> x, std::optional<double> y) {
	if (!index.has_value() || !x.has_value() || !y.has_value()) {
		throw Error("Missing required parameter in any node definition");
	} 
	if (index < 0) {
		throw Error("Index is lower tnan zero");
	}
}

void Node::setIndex(int index) { this->index = index; }

void Node::setX(double x) { this->x = x; }

void Node::setY(double y) { this->y = y; }

void Node::setDispX(double disp_x) { this->disp_x = disp_x; }

void Node::setDispY(double disp_y) { this->disp_y = disp_y; }

int Node::getIndex() const { return this->index; }

double Node::getX() const { return this->x; }

double Node::getY() const { return this->y; }

double Node::getDispX() const { return this->disp_x; }

double Node::getDispY() const { return this->disp_y; }

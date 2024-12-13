#pragma once
#include <optional>

class Node {
private:
	int index;
	double x, y, disp_x = 0, disp_y = 0;
public:
	Node(std::optional<int> index, std::optional<double> x, std::optional<double> y);

	void checkParameters(std::optional<int> index, std::optional<double> x, std::optional<double> y);

	int getIndex() const;
	void setIndex(int i);

	double getX() const;
	void setX(double x);

	double getY() const;
	void setY(double y);

	double getDispX() const;
	void setDispX(double disp_x);

	double getDispY() const;
	void setDispY(double disp_y);
};
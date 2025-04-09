#pragma once
#include <optional>
#include <memory>
#include <unordered_map>
#include <string>

class Node {
private:
	int index;
	double x;
	double y;
	double disp_x = 0;
	double disp_y = 0;
public:
	Node(int index, double x, double y);

	static std::shared_ptr<Node> createNode(std::unordered_map<std::string, std::optional<double>> node_data);

	static void checkParameters(std::optional<int> index, std::optional<double> x, std::optional<double> y);

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
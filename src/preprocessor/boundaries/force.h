#pragma once
#include <optional>
#include <unordered_map>

class Force {
   private:
    int index;
    double force_x;
    double force_y;

   public:
    Force(int i, double x, double y);

    static Force createForce(std::unordered_map<std::string, std::optional<double>> force_components);

    static void checkParameters(std::optional<int> i, std::optional<double> x, std::optional<double> y);

    int getIndex() const;
    double getForceX() const;
    double getForceY() const;
};

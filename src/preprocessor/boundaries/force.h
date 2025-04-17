#pragma once
#include <optional>
#include <unordered_map>

class Force
{
private:
    int index;
    float force_x;
    float force_y;
public:
    Force(int i, float x, float y);

    static Force createForce(std::unordered_map<std::string, std::optional<double>> force_components);
    
    static void checkParameters(std::optional<int> i, std::optional<float> x, std::optional<float> y);
    
    int getIndex() const;
    float getForceX() const;
    float getForceY() const;
};

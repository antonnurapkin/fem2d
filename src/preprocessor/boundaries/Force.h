#pragma once
#include <optional>

class Force
{
private:
    int index;
    float force_x, force_y;
public:
    Force(std::optional<int> i, std::optional<float> x, std::optional<float> y);
    void checkParameters(std::optional<int> i, std::optional<float> x, std::optional<float> y);
    
    // Геттеры
    int getIndex() const;
    float getForceX() const;
    float getForceY() const;

    // Сеттеры
    void setIndex(int i);
    void setForceX(float x);
    void setForceY(float y);

};

#pragma once
#include <optional>

class Force
{
private:
    int index;
    float force_x, force_y;
public:
    Force(int i, float x, float y);

    static Force createForce(std::optional<int> i, std::optional<float> x, std::optional<float> y);
    
    static void checkParameters(std::optional<int> i, std::optional<float> x, std::optional<float> y);
    
    // �������
    int getIndex() const;
    float getForceX() const;
    float getForceY() const;

    // �������
    void setIndex(int i);
    void setForceX(float x);
    void setForceY(float y);

};

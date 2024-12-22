#pragma once
#include <optional>

class Support
{
private:
    int index;
    std::optional<double> disp_x, disp_y;
public:
    // �����������
    Support(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y);
    void checkParameters(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y);

    // �������
    int getIndex() const;
    std::optional<double> getDispX() const;
    std::optional<double> getDispY() const;

    // �������
    void setIndex(int index);
    void setDispX(std::optional<double> disp_x);
    void setDispY(std::optional<double> disp_y);
};

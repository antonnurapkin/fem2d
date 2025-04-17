#pragma once
#include <optional>
#include <unordered_map>
#include <string>

class Support
{
private:
    int index;
    std::optional<double> disp_x;
    std::optional<double> disp_y;
public:
    Support(int index, std::optional<double> disp_x, std::optional<double> disp_y);

    static Support createSupport(std::unordered_map<std::string, std::optional<double>> support_components);

    static void checkParameters(std::optional<int> index, std::optional<double> disp_x, std::optional<double> disp_y);

    int getIndex() const;
    std::optional<double> getDispX() const;
    std::optional<double> getDispY() const;

    void setIndex(int index);
    void setDispX(std::optional<double> disp_x);
    void setDispY(std::optional<double> disp_y);
};

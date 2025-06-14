#pragma once
#include <optional>
#include <string>
#include <unordered_map>

class Material {
   private:
    double mu;
    double density;
    double Emod;
    double index;

   public:
    Material(double mu, double Emod, double density, int index);

    static Material createMaterial(std::unordered_map<std::string, std::optional<double>> material_data);

    static void checkParameters(std::optional<double> Emod, std::optional<double> mu, std::optional<double> density, std::optional<int> index);

    double getEmod() const;
    double getMu() const;
    double getDensity() const;
    int getIndex() const;
};

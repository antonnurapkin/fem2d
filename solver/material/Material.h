#pragma once
#include <optional>

class Material {
private:
	double mu, density, Emod, index;
public:
	Material(
		std::optional<double> P_Emod, 
		std::optional<double> p_mu,
		std::optional<double> p_density,
		std::optional<int> index
	);

	void checkParameters(
		std::optional<double> Emod, 
		std::optional<double> mu, 
		std::optional<double> density, 
		std::optional<int> index
	);

	double getEmod() const;
	double getMu() const;
	double getDensity() const;
	int getIndex() const;

	void setEmod(double p_Emod);
	void setMu(double p_mu);
	void setDensity(double p_density);
	void setIndex(int index);
};

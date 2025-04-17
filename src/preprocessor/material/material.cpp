#include <iostream>
#include <unordered_map>
#include "../error.h"
#include "material.h"

Material::Material(double mu, double density, double Emod, int index) : mu(mu), density(density), Emod(Emod), index(index) {};

Material Material::createMaterial(std::unordered_map<std::string, std::optional<double>> material_data) {
	Material::checkParameters(material_data["mu"], material_data["density"], material_data["Emod"], material_data["index"]);

	return Material( 
		material_data["mu"].value(), 
		material_data["density"].value(),
		material_data["Emod"].value(), 
		material_data["index"].value()
	);
}

void Material::checkParameters(std::optional<double> mu, std::optional<double> density, std::optional<double> Emod, std::optional<int> index) {
	if (!mu.has_value() || !Emod.has_value() || !density.has_value() || !index.has_value()) {
		throw PreprocessorError("Missing required parameter in material definition");
	}
	if (mu > 1 || mu < 0) {
		throw PreprocessorError("Mu is out of range");
	}
	if (Emod < 0) {
		throw PreprocessorError("Emod is out of range");
	}
	if (density < 0) {
		throw PreprocessorError("Density is out of range");
	}
	if (index < 0) {
		throw PreprocessorError("Index is out of range");
	}
}

double Material::getMu() const {
	return mu;
}

double Material::getEmod() const {
	return Emod;
}

double Material::getDensity() const {
	return density;
}

int Material::getIndex() const  {
	return this->index;
}
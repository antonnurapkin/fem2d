#include <iostream>
#include "../utils/Error.h"
#include "Material.h"

Material::Material(double mu, double density, double Emod, int index) : mu(mu), density(density), Emod(Emod), index(index) {};

Material Material::createMaterial(std::optional<double> Emod, std::optional<double> mu, std::optional<double> density, std::optional<int> index) {
	Material::checkParameters(mu, density, Emod, index);

	return Material(mu.value(), density.value(), Emod.value(), index.value());
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

void Material::setMu(double mu) {
	if (0 < mu < 1) {
		this->mu = mu;
	}
	else {
		throw PreprocessorError("Mu is out of range\n");
	}
}

void Material::setEmod(double Emod) {
	if (Emod > 0) {
		this->Emod = Emod;
	}
	else {
		throw PreprocessorError("Emod is lower than zero\n");
	}
}

void Material::setDensity(double density) {
	if (density > 0) {
		this->density = density;
	}
	else {
		throw PreprocessorError("Density is lower than zero\n");
	}
}

void Material::setIndex(int index) {
	if (index > 0) {
		this->index = index;
	}
	else {
		throw PreprocessorError("Index is lower than zero\n");
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
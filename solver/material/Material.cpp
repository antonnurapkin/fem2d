#include <iostream>
#include "Material.h"
#include "Error.h"

Material::Material(std::optional<double> Emod, std::optional<double> mu, std::optional<double> density, std::optional<int> index) {
	Material::checkParameters(Emod, mu, density, index);
	this->mu = mu.value();
	this->Emod = Emod.value();
	this->density = density.value();
	this->index = index.value();
}

void Material::checkParameters(std::optional<double> Emod, std::optional<double> mu, std::optional<double> density, std::optional<int> index) {
	if (!mu.has_value() || !Emod.has_value() || !density.has_value() || !index.has_value()) {
		throw Error("Missing required parameter in material definition");
	}
	if (mu > 1 || mu < 0) {
		throw Error("Mu is out of range");
	}
	if (Emod < 0) {
		throw Error("Emod is out of range");
	}
	if (density < 0) {
		throw Error("Density is out of range");
	}
	if (index < 0) {
		throw Error("Index is out of range");
	}
}

void Material::setMu(double mu) {
	if (0 < mu < 1) {
		this->mu = mu;
	}
	else {
		throw Error("Mu is out of range\n");
	}
}

void Material::setEmod(double Emod) {
	if (Emod > 0) {
		this->Emod = Emod;
	}
	else {
		throw Error("Emod is lower than zero\n");
	}
}

void Material::setDensity(double density) {
	if (density > 0) {
		this->density = density;
	}
	else {
		throw Error("Density is lower than zero\n");
	}
}

void Material::setIndex(int index) {
	if (index > 0) {
		this->index = index;
	}
	else {
		throw Error("Index is lower than zero\n");
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
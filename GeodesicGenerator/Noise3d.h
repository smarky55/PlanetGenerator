#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "xxhash.h"

class Noise3d {
	unsigned long long Seed;
	double Wavelength;
	
	static double fade(double t);
	static double lerp(double a, double b, double x);
	static double grad(size_t hash, double x, double y, double z);
	size_t getHash(long long x, long long y, long long z);
	static size_t getHash(long long x, long long y, long long z, size_t seed);
public:
	Noise3d(unsigned long long seed, double wavelength);
	~Noise3d();

	double getPoint(double x, double y, double z);
	static double getPoint(size_t seed, double wavelength, double x, double y, double z);
	static glm::vec3 getGrad(size_t seed, double wavelength, double x, double y, double z);
	static double getPoint(size_t seed, double wavelength, glm::vec3& vertex);
};


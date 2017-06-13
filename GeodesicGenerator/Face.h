#pragma once

#include <glm/glm.hpp>

class Face {
	unsigned Indices[3];
public:
	Face(unsigned i0, unsigned i1, unsigned i2);
	~Face();

	unsigned& operator[](std::size_t idx) { return Indices[idx]; }
	const unsigned& operator[](std::size_t idx) const { return Indices[idx]; }
};


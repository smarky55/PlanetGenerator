#pragma once
#include <vector>
#include <iostream>
#include "Face.h"

class Geodesic {
	std::vector<Face> Faces;
	
	void tesselate(unsigned face, unsigned depth);
	void genIndices();
	void genNormals();
protected:
	void init(std::vector<glm::vec3>& verts, std::vector<unsigned>& inds, unsigned depth);
public:
	Geodesic(std::vector<glm::vec3>& verts, std::vector<unsigned>& inds, unsigned depth);
	Geodesic();
	~Geodesic();

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<unsigned> Indices;
};


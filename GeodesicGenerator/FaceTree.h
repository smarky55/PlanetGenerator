#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Noise3d.h"

struct Mesh {
	Mesh(size_t seed);
	~Mesh();

	size_t Seed;

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec4> Colours;
	std::vector<glm::vec3> Normals;

	void addVertex(glm::vec3 vertex);
	size_t size() { return Vertices.size(); }
};

class FaceTree {
	unsigned Indices[3];
	Mesh* MMesh;

	FaceTree* FaceA = nullptr;
	FaceTree* FaceB = nullptr;
	FaceTree* FaceC = nullptr;
	FaceTree* FaceD = nullptr;

	void tesselate();
public:
	FaceTree(Mesh* mesh, unsigned i1, unsigned i2, unsigned i3);
	~FaceTree();

	glm::vec3 FaceNormal;

	void getFaces(std::vector<FaceTree*>& faces, unsigned depth);

	unsigned& operator[](std::size_t idx) { return Indices[idx]; }
	const unsigned& operator[](std::size_t idx) const { return Indices[idx]; }
};


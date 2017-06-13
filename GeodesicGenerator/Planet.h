#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "Geodesic.h"
#include "Noise3d.h"
#include "controls.h"
#include "Camera.h"
#include "FaceTree.h"

class Planet{
	GLuint VertexBuffer, ColourBuffer, NormalBuffer, IndexBuffer;
	std::vector<FaceTree*> Faces;
	void genIndices(unsigned depth);
public:
	Planet(size_t seed, unsigned depth);
	~Planet();

	Mesh mesh;

	//std::vector<glm::vec3> Vertices;
	//std::vector<glm::vec3> Colours;
	//std::vector<glm::vec3> Normals;
	std::vector<unsigned> Indices;

	void draw(GLuint programID, Camera& camera);
};


#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "Camera.h"
#include "FaceTree.h"
#include "shaders.h"

class Planet{
	GLuint VertexBuffer, ColourBuffer, NormalBuffer, IndexBuffer, AtmoIndBuffer;
	size_t Seed;
	std::vector<FaceTree*> Faces;
	GLuint planetTexture;
	
	void genIndices(unsigned depth);
	void genTexture();
public:
	Planet(size_t seed, unsigned depth=0);
	~Planet();

	Mesh mesh;

	//std::vector<glm::vec3> Vertices;
	//std::vector<glm::vec3> Colours;
	//std::vector<glm::vec3> Normals;
	std::vector<unsigned> Indices;
	std::vector<unsigned> AtmoIndices;

	void draw(GLuint programID, Camera* camera);
};


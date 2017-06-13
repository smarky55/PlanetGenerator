#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include "Geodesic.h"
#include "Noise3d.h"
#include "controls.h"
#include "Camera.h"

class Planet: public Geodesic {
	GLuint VertexBuffer, ColourBuffer, IndexBuffer;
	void genBuffers();
public:
	Planet(size_t seed, unsigned depth);
	~Planet();

	std::vector<glm::vec3> Colours;

	void draw(GLuint programID, Camera& camera);
};


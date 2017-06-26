#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "Camera.h"
#include "FaceTree.h"
#include "shaders.h"

struct sCubeMapProps {
	GLuint *texture;
	const char* vertex_file_path;
	const char* fragment_file_path;
	GLint internal_format = GL_RGBA8;
	GLint format = GL_RGBA;
	GLsizei texture_size = 1024;
};

class Planet{
	GLuint VertexBuffer, ColourBuffer, NormalBuffer, IndexBuffer, AtmoIndBuffer;
	size_t Seed;
	std::vector<FaceTree*> Faces;
	GLuint planetTexture, heightMap, normalMap;
	
	void genIndices(unsigned depth);
	void genTexture(sCubeMapProps properties);
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


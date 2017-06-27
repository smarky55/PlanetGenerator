#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class ShaderProgram {
	std::vector<GLuint> shaderStages;
	bool hasLinked = false;
	void loadFile(const char * file_path, std::string& code);
	const char * stageName(GLenum stage);
public:
	GLint programID;
	ShaderProgram();
	~ShaderProgram();

	void addStage(GLenum stage, const char * file_path);
	void linkProgram();
};


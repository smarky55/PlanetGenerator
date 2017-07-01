#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

void loadFile(const char * file_path, std::string& code);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);


class ShaderProgram {
	std::vector<GLuint> shaderStages;
	bool hasLinked = false;
	void loadFile(const char * file_path, std::string& code, std::vector<std::string>& lines);
	const char * stageName(GLenum stage);
	void printErrors(GLenum stage, std::vector<char>& errorMessage, std::vector<std::string>& lines);
public:
	GLint programID;
	ShaderProgram();
	~ShaderProgram();

	void addStage(GLenum stage, const char * file_path);
	void linkProgram();
};


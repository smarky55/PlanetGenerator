#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "ArxLoader.h"

class ShaderProgram {
	std::vector<GLuint> shaderStages;
	ArxLoader * Archive = nullptr;
	bool hasLinked = false;
	void loadFile(const char * file_path, std::string& code, std::vector<std::string>& lines);
	void loadFile(const char * asset_path, ArxLoader * archive, std::string& code, std::vector<std::string>& lines);
	const char * stageName(GLenum stage);
	void printErrors(GLenum stage, std::vector<char>& errorMessage, std::vector<std::string>& lines);
public:
	GLint programID;
	bool isLinked() { return hasLinked; }
	ShaderProgram();
	ShaderProgram(ArxLoader * archive);
	~ShaderProgram();

	void addStage(GLenum stage, const char * file_path);
	void linkProgram();
};


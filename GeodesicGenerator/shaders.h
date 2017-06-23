#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

void loadFile(const char * file_path, std::string& code);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);


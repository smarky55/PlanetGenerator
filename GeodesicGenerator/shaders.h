#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

void loadFile(const char * file_path, std::string& code) {
	std::ifstream stream(file_path, std::ios::in);
	code = "";
	if(stream.is_open()) {
		std::string line = "";
		while(getline(stream, line)) {
			size_t include_pos = line.find("#include");
			if(include_pos != std::string::npos) {
				size_t gt_pos, lt_pos;
				gt_pos = line.find("<", include_pos + 8);
				lt_pos = line.find(">", gt_pos);
				loadFile(line.substr(gt_pos + 1, lt_pos - (gt_pos + 1)).c_str(), line);
				code += line;
			} else {
				//std::cout << line << std::endl;
				code += line + "\n";
			}
		}
		stream.close();
	} else {
		throw std::runtime_error("Failed to open file: " + std::string(file_path));
	}
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	/*std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()) {
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	} else {
		throw std::runtime_error("Failed to open vertex shader: " + std::string(vertex_file_path));
	}*/

	loadFile(vertex_file_path, VertexShaderCode);

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	/*std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()) {
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	} else {
		throw std::runtime_error("Failed to open fragment shader: " + std::string(fragment_file_path));
	}*/

	loadFile(fragment_file_path, FragmentShaderCode);

	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(Result == GL_FALSE && InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("Vertex Shader Errors:\n%s\n", &VertexShaderErrorMessage[0]);
		throw std::runtime_error("Vertex Shader Errors : \n " + std::string(&VertexShaderErrorMessage[0]) + '\n');
	}



	// Compile Fragment Shader
	//printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	Result = GL_FALSE;
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(Result == GL_FALSE && InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("Fragment Shader Errors:\n%s\n", &FragmentShaderErrorMessage[0]);
		throw std::runtime_error("Fragment Shader Errors:\n" + std::string(&FragmentShaderErrorMessage[0]) + '\n');
	}



	// Link the program
	//printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	Result = GL_FALSE;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(Result == GL_FALSE && InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("Program Errors:\n%s\n", &ProgramErrorMessage[0]);
		throw std::runtime_error("Shader program error: \n" + std::string(&ProgramErrorMessage[0]) + '\n');
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	//printf("Program OK!\n");

	return ProgramID;
}

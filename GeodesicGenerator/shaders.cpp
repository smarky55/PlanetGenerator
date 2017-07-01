#include "shaders.h"

void ShaderProgram::loadFile(const char * file_path, std::string & code) {
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

const char * ShaderProgram::stageName(GLenum stage) {
	switch(stage) {
	case GL_VERTEX_SHADER:
		return "Vertex Shader";
	case GL_GEOMETRY_SHADER:
		return "Geometry Shader";
	case GL_FRAGMENT_SHADER:
		return "Fragment Shader";
	default:
		return "Unknown Stage";
	}
}

ShaderProgram::ShaderProgram() {
	programID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programID);
}

void ShaderProgram::addStage(GLenum stage, const char * file_path) {
	if(!hasLinked) {
		GLuint shaderID = glCreateShader(stage);

		std::string shaderCode;

		loadFile(file_path, shaderCode);

		const char * source = shaderCode.c_str();
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);

		GLint result = GL_FALSE;
		int infoLogLength;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(result == GL_FALSE && infoLogLength > 0) {
			std::vector<char> errorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
			printf("%s Errors:\n%s\n", stageName(stage), &errorMessage[0]);
			throw std::runtime_error("Error when compiling shader");
		}

		shaderStages.push_back(shaderID);
	} else {
		throw std::runtime_error("Cannot add shader stage to already linked program.");
	}
}

void ShaderProgram::linkProgram() {
	if(!hasLinked) {
		for each (GLuint shaderID in shaderStages) {
			glAttachShader(programID, shaderID);
		}

		glLinkProgram(programID);

		GLint result = GL_FALSE;
		int infoLogLength;
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(result == GL_FALSE && infoLogLength > 0) {
			std::vector<char> errorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
			printf("Program Errors:\n%s\n", &errorMessage[0]);
			throw std::runtime_error("Error when linking shader");
		}

		hasLinked = true;

		for each (GLuint shaderID in shaderStages) {
			glDeleteShader(shaderID);
		}
	} else {
		throw std::runtime_error("Cannot link already linked program.");
	}
}

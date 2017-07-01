#include "shaders.h"


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

void ShaderProgram::loadFile(const char * file_path, std::string & code, std::vector<std::string>& lines) {
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
				std::vector<std::string> includeLines;
				loadFile(line.substr(gt_pos + 1, lt_pos - (gt_pos + 1)).c_str(), line, includeLines);
				code += line;
				lines.insert(lines.end(), includeLines.begin(), includeLines.end());
			} else {
				//std::cout << line << std::endl;
				code += line + "\n";
				lines.push_back(line);
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

void ShaderProgram::printErrors(GLenum stage, std::vector<char>& errorMessage, std::vector<std::string>& lines) {
	std::string errors = std::string(errorMessage.begin(), errorMessage.end());
	std::regex r1("\\(\\d+\\)");
	std::regex r2("\\d\\(\\d+\\)[^\n]*");
	std::sregex_iterator it1(errors.begin(), errors.end(), r1);
	std::sregex_iterator it2(errors.begin(), errors.end(), r2);
	std::sregex_iterator it_end;
	printf("%s Errors:\n", stageName(stage));
	while(it1 != it_end && it2 != it_end) {
		size_t index = stoi(errors.substr((*it1).position()+1,(*it1).length()-2))-1;
		std::cout << (*it2).str() << std::endl;
		for(auto i = lines.begin()+(index-2); i != lines.end() && i !=lines.begin()+(index+2); i++) {
			std::cout << std::distance(lines.begin(), i)+1 <<  *i << std::endl;
		
		}
		it1++;
		it2++;
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
		std::vector<std::string> shaderLines;
		loadFile(file_path, shaderCode, shaderLines);

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
			//printf("%s Errors:\n%s\n", stageName(stage), &errorMessage[0]);
			printErrors(stage, errorMessage, shaderLines);
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

		for each (GLuint shaderID in shaderStages) {
			glDeleteShader(shaderID);
		}
	} else {
		throw std::runtime_error("Cannot link already linked program.");
	}
}

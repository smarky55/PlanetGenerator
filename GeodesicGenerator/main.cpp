
#include <iostream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders.h"
#include "Planet.h"
#include "OrbitCamera.h"

#define WINDOWS_APP

/* typedef glm::vec3 vec3;
const double ic_a = 0.52573111211913360602566908484789;
const double ic_b = 0.85065080835203993218154049706302;
std::vector<vec3> icos = {vec3(0, ic_a, ic_b), vec3(0, ic_a, -ic_b), vec3(0, -ic_a, ic_b), vec3(0, -ic_a, -ic_b),
								vec3(ic_a, ic_b, 0), vec3(ic_a, -ic_b, 0), vec3(-ic_a, ic_b, 0), vec3(-ic_a, -ic_b, 0),
								vec3(ic_b, 0, ic_a), vec3(-ic_b, 0, ic_a), vec3(ic_b, 0, -ic_a), vec3(-ic_b, 0, -ic_a),};
std::vector<vec3> icCol = {vec3(0.2,0.0,0.0),vec3(0.4,0,0),vec3(0.6,0,0),vec3(0.8,0,0)
								,vec3(1,0,0),vec3(1,0.2,0),vec3(1,0.4,0),vec3(1,0.6,0)
								,vec3(1,0.8,0),vec3(1.0,1.0,0),vec3(1.0,1,0.2),vec3(1,1,0.4)};
std::vector<unsigned> isInd = {
	0,4,6,
	0,8,4,
	0,6,9,
	0,2,8,
	0,9,2,
	1,6,4,
	1,11,6,
	1,4,10,
	1,3,11,
	1,10,3,
	4,8,10,
	6,11,9,
	5,8,2,
	5,10,8,
	5,3,10,
	5,7,3,
	5,2,7,
	3,7,11,
	7,9,11,
	7,2,9
}; */

#ifdef _DEBUG
void APIENTRY debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	// Some debug messages are just annoying informational messages
	switch(id) {
	case 131169: // The driver allocated storage for renderbuffer
	case 131185: // glBufferData
		return;
	}

	printf("Message: %s\n", message);
	printf("Source: ");

	switch(source) {
	case GL_DEBUG_SOURCE_API:
		printf("API");
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		printf("Window System");
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		printf("Shader Compiler");
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		printf("Third Party");
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		printf("Application");
		break;
	case GL_DEBUG_SOURCE_OTHER:
		printf("Other");
		break;
	}

	printf("\n");
	printf("Type: ");

	switch(type) {
	case GL_DEBUG_TYPE_ERROR:
		printf("Error");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		printf("Deprecated Behaviour");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("Undefined Behaviour");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		printf("Portability");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("Performance");
		break;
	case GL_DEBUG_TYPE_MARKER:
		printf("Marker");
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		printf("Push Group");
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		printf("Pop Group");
		break;
	case GL_DEBUG_TYPE_OTHER:
		printf("Other");
		break;
	}

	printf("\n");
	printf("ID: %d\n", id);
	printf("Severity: ");

	switch(severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		printf("High");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		printf("Medium");
		break;
	case GL_DEBUG_SEVERITY_LOW:
		printf("Low");
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		printf("Notification");
		break;
	}

	printf("\n\n");
}
#endif // _DEBUG


Camera* camera;
Planet* geo;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if(key == GLFW_KEY_R && action == GLFW_PRESS) {
		delete geo;
		int seed = rand();
		std::cout << seed << std::endl;
		geo = new Planet(seed, 6);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	static_cast<OrbitCamera*>(camera)->distanceOffset(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

#ifndef _DEBUG
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {
#else 
int main() {
#endif // WINDOWS_APP
	if(!glfwInit()) {
		throw std::runtime_error("Failed to init GLFW!");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // _DEBUG

	GLFWwindow * window;
	int width = 1600;
	int height = 900;
	window = glfwCreateWindow(width, height, "Terrain View", NULL, NULL);

	if(window == NULL) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window!");
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialise glew!");
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugMessage, nullptr);
#endif // _DEBUG

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

	glfwSetKeyCallback(window, key_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	ShaderProgram mainProgram = ShaderProgram();
	mainProgram.addStage(GL_VERTEX_SHADER, "planet_vertex.glsl");
	mainProgram.addStage(GL_FRAGMENT_SHADER, "planet_fragment.glsl");
	mainProgram.linkProgram();

	/*float cubeVertices[] = {
		-1, -1, -1,
		-1, -1, 1,
		-1, 1, -1,
		-1, 1, 1,
		1, -1, -1,
		1, -1, 1,
		1, 1, -1,
		1, 1, 1
	};
	float cubeColours[] = {
		0.5,0.5,0,
		1,0,0,
		0,0.5,0.5,
		0,1,0,
		1,1,1,
		0.5,0.5,0,
		0,0,1,
		0.5,0.5,0.5
	};
	unsigned cubeIndices[] = {
		0,1,2,
		1,3,2,
		4,6,5,
		5,6,7,
		0,4,1,
		1,4,5,
		2,3,6,
		3,7,6,
		0,2,4,
		2,6,4,
		1,5,3,
		3,5,7
	};*/

	geo = new Planet(76, 4);

	OrbitCamera cam = OrbitCamera(glm::vec3(0));
	camera = &cam;

	glfwSetScrollCallback(window, scroll_callback);

	double lastTime, currentTime, deltaTime;
	double acc = 0;
	int nFrames = 0;
	lastTime = glfwGetTime();

	srand(time(NULL));

	while(!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		acc += deltaTime;
		nFrames++;
		if(acc > 1) {
			std::cout << nFrames / acc << ' ' << acc / nFrames << std::endl;
			acc = 0;
			nFrames = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glUseProgram(programID);
		
		camera->update();

		geo->draw(mainProgram.programID, camera);
		//geo->drawNormals(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete geo;

	return 0;
}
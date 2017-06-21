
#include <iostream>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <cstdlib>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders.h"
#include "Planet.h"
#include "OrbitCamera.h"


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


Camera* camera;
Planet* geo;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if(key == GLFW_KEY_R && action == GLFW_PRESS) {
		delete geo;
		geo = new Planet(rand(), 6);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	static_cast<OrbitCamera*>(camera)->distanceOffset(yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {	
	if(!glfwInit()) {
		throw std::runtime_error("Failed to init GLFW!");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

	glfwSetKeyCallback(window, key_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//GLuint programID = LoadShaders("basic_vertex.glsl", "basic_fragment.glsl");
	//GLuint programID = LoadShaders("shading_vertex.glsl", "shading_fragment.glsl");
	GLuint programID = LoadShaders("planet_vertex.glsl", "planet_fragment.glsl");

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

	geo = new Planet(76,1);

	OrbitCamera cam = OrbitCamera(glm::vec3(0));
	camera = &cam;

	glfwSetScrollCallback(window, scroll_callback);

	double lastTime, currentTime, deltaTime;
	double acc = 0;
	int nFrames = 0;
	lastTime = glfwGetTime();

	srand(lastTime);

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

		glUseProgram(programID);
		
		camera->update();

		geo->draw(programID, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete geo;

	return 0;
}
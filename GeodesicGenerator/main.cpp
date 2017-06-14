
#include <iostream>
#include <vector>
#include <iostream>
#include <Windows.h>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders.h"
#include "controls.h"
#include "Noise3d.h"
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	static_cast<OrbitCamera*>(camera)->distanceOffset(yoffset*-0.2);
}

//void tesselate(std::vector<glm::vec3>& vertices, std::vector<unsigned>& indices, size_t face, unsigned depth = 0) {
//	unsigned end = vertices.size();
//	unsigned ind[] = {end, indices[3 * face], end + 1, end + 1, indices[3 * face + 1], end + 2, end + 2, indices[3 * face + 2], end, end, end + 1, end + 2};
//	if(depth != 0) {
//		glm::vec3 v1, v2, v3, a, b, c;
//		v1 = vertices[indices[3*face]];
//		v2 = vertices[indices[3*face + 1]];
//		v3 = vertices[indices[3*face + 2]];
//		indices.erase(indices.begin() + 3 * face, indices.begin() + 3 * face + 3);
//
//		vertices.push_back(glm::normalize((v1 + v3) / 2.0f));
//		vertices.push_back(glm::normalize((v1 + v2) / 2.0f));
//		vertices.push_back(glm::normalize((v2 + v3) / 2.0f));
//		size_t nface = indices.size() / 3;
//		for(auto i: ind) {
//			indices.push_back(i);
//		}
//		for(size_t i = 0; i < 4; i++) {
//			tesselate(vertices, indices, nface, depth - 1);
//		}
//	}
//}

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
	int width = 1024;
	int height = 786;
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

	//Geodesic geo = Geodesic(icos, isInd, 6);
	Planet geo = Planet(76,6);

	/*for(size_t i = 0; i < 20; i++) {
		tesselate(icos, isInd, 0, 6);
	}*/

	Noise3d terrain = Noise3d(1, 0.5);
	Noise3d t2 = Noise3d(2, 0.1);

	//icCol.clear();
	//icCol.resize(geo.Vertices.size());
	//for(size_t i = 0; i < geo.Vertices.size(); i++) {
	//	vec3 vert = geo.Vertices[i];
	//	double point = (Noise3d::getPoint(1, 1, vert.x, vert.y, vert.z)*0.5 
	//					+ Noise3d::getPoint(2, 0.5, vert.x, vert.y, vert.z)*0.5 
	//					+ Noise3d::getPoint(3, 0.25, vert.x, vert.y, vert.z)*0.25
	//					+ Noise3d::getPoint(4, 0.1, vert.x, vert.y, vert.z)*0.1) / 1.35;
	//	// std::cout << point << std::endl;
	//	vec3 colour = vec3(0, (102. / 255)*point, 0);
	//	icCol[i] = colour;
	//	geo.Vertices[i] = vert * (float)(1 + point/10);
	//}


	/* GLuint VertexBuffer, ColourBuffer, IndexBuffer;

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, geo.Vertices.size() * sizeof(glm::vec3), geo.Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ColourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
	glBufferData(GL_ARRAY_BUFFER, geo.Colours.size() * sizeof(glm::vec3), geo.Colours.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, geo.Indices.size() * sizeof(unsigned), geo.Indices.data(), GL_STATIC_DRAW);
	*/

	//glfwSetCursorPos(window, 1024 / 2, 786 / 2);
	//Camera camera = Camera();
	OrbitCamera cam = OrbitCamera(glm::vec3(0));
	camera = &cam;

	glfwSetScrollCallback(window, scroll_callback);

	double lastTime, currentTime, deltaTime;
	double acc = 0;
	int nFrames = 0;
	lastTime = glfwGetTime();

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
		
		//computeMatrices();
		camera->update();

		/* glm::mat4 model = glm::rotate(-acos(glm::dot(geo.Vertices[0], glm::vec3(0,1,0))), glm::vec3(1,0,0)) * glm::translate(glm::vec3(0));
		glm::mat4 view = getViewMatrix();
		glm::mat4 projection = getProjectionMatrix();
		glm::mat4 MVP = projection * view * model;

		GLuint vertPosID, colourPosID, mvpID;
		vertPosID = glGetAttribLocation(programID, "vertex_position");
		colourPosID = glGetAttribLocation(programID, "vertex_colour");
		mvpID = glGetUniformLocation(programID, "MVP");

		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(vertPosID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(vertPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(colourPosID);
		glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
		glVertexAttribPointer(colourPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

		//glDrawArrays(GL_POINTS, 0, icos.size());

		glDrawElements(GL_TRIANGLES, geo.Indices.size(), GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(vertPosID);
		glDisableVertexAttribArray(colourPosID);*/

		geo.draw(programID, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
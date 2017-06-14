#pragma once

//#include <iostream>

#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
protected:
	GLFWwindow* Window;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::vec3 Position;
	float HorizontalAngle;
	float VerticalAngle;
	float FoV;
	float BaseSpeed;
	float MouseSpeed;
public:
	Camera();
	Camera(glm::vec3 position, float horizontalAngle, float verticalAngle, float fov, float baseSpeed, float mouseSpeed);
	~Camera();

	virtual void update();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition() { return Position; }
};


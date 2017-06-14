#include "OrbitCamera.h"



void OrbitCamera::updatePosition() {
	Position.x = Distance * glm::sin(HorizontalAngle) * glm::cos(VerticalAngle);
	Position.z = Distance * glm::cos(HorizontalAngle) * glm::cos(VerticalAngle);
	Position.y = Distance * glm::sin(-VerticalAngle);
}

OrbitCamera::OrbitCamera(glm::vec3 center) {
	HorizontalAngle = 0.0f;
	VerticalAngle = 0.0f;
	Distance = 2.0f;

	MouseSpeed /= 2;

	Center = center;

	updatePosition();
}


OrbitCamera::~OrbitCamera() {
}

void OrbitCamera::update() {
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	int width, height;
	glfwGetWindowSize(Window, &width, &height);

	double xPos, yPos;
	glfwGetCursorPos(Window, &xPos, &yPos);
	glfwSetCursorPos(Window, width / 2., height / 2.);
	
	HorizontalAngle += MouseSpeed * float(width / 2. - xPos);
	HorizontalAngle = std::fmod(HorizontalAngle, 3.14f * 2);
	VerticalAngle += MouseSpeed * float(height / 2. - yPos);
	VerticalAngle = glm::clamp(VerticalAngle, -1.57f, 1.57f);

	HorizontalAngle += MouseSpeed * float(width / 2. - xPos);
	HorizontalAngle = std::fmod(HorizontalAngle, 3.14f * 2);
	VerticalAngle += MouseSpeed * float(height / 2. - yPos);
	VerticalAngle = glm::clamp(VerticalAngle, -1.57f, 1.57f);

	glm::vec3 up = glm::vec3(0, 1, 0);

	updatePosition();

	View = glm::lookAt(Position, Center, up);

	Projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
}



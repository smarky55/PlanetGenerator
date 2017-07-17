#include "OrbitCamera.h"

void OrbitCamera::updatePosition() {
	Position.x = Distance * glm::sin(HorizontalAngle) * glm::cos(VerticalAngle);
	Position.z = Distance * glm::cos(HorizontalAngle) * glm::cos(VerticalAngle);
	Position.y = Distance * glm::sin(-VerticalAngle);
}

OrbitCamera::OrbitCamera(glm::vec3 center) {
	HorizontalAngle = 0.0f;
	VerticalAngle = 0.0f;
	Distance = 2.5f;

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
	static double lastX = -1, lastY = -1;
	if(lastX == -1 || lastY == -1) {
		glfwGetCursorPos(Window, &lastX, &lastY);
	}
	double xPos, yPos;
	glfwGetCursorPos(Window, &xPos, &yPos);
	//glfwSetCursorPos(Window, width / 2., height / 2.);
	double dx = lastX - xPos;
	double dy = lastY - yPos;

	if(glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		HorizontalAngle += MouseSpeed * glm::min((Distance - 1.0) / 2, 1.0) * float(dx);// glm::floor(width / 2.0) - xPos);
		HorizontalAngle = std::fmod(HorizontalAngle, 3.14f * 2);
		VerticalAngle += MouseSpeed * glm::min((Distance - 1.0) / 2, 1.0) * float(dy);// glm::floor(height / 2.0) - yPos);
		VerticalAngle = glm::clamp(VerticalAngle, -1.57f, 1.57f);
	}

	glm::vec3 up = glm::vec3(0, 1, 0);

	updatePosition();

	View = glm::lookAt(Position, Center, up);

	Projection = glm::perspective(FoV, float(width) / float(height), 0.1f, 100.0f);
	

	lastX = xPos;
	lastY = yPos;
}



#include "Camera.h"



Camera::Camera() {
	Window = glfwGetCurrentContext();
	Position = glm::vec3(0.5, 1, 1);
	HorizontalAngle = 3.14f;
	VerticalAngle = -0.2f;
	FoV = 45.0f;
	BaseSpeed = 3.0f;
	MouseSpeed = 0.005f;

	int width, height;
	glfwGetWindowSize(Window, &width, &height);

	glfwSetCursorPos(Window, width / 2., height / 2.);
}

Camera::Camera(glm::vec3 position, float horizontalAngle, float verticalAngle, float fov, float baseSpeed, float mouseSpeed) {
	Window = glfwGetCurrentContext();
	Position = position;
	HorizontalAngle = horizontalAngle;
	VerticalAngle = verticalAngle;
	FoV = fov;
	BaseSpeed = baseSpeed;
	MouseSpeed = mouseSpeed;

	int width, height;
	glfwGetWindowSize(Window, &width, &height);

	glfwSetCursorPos(Window, width / 2., height / 2.);
}


Camera::~Camera() {
}

void Camera::update() {
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

	glm::vec3 direction = glm::vec3(
		cos(VerticalAngle) * sin(HorizontalAngle),
		sin(VerticalAngle),
		cos(VerticalAngle) * cos(HorizontalAngle)
	);
	glm::vec3 right = glm::vec3(
		sin(HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(HorizontalAngle - 3.14f / 2.0f)
	);
	glm::vec3 up = glm::cross(right, direction);

	float speed = BaseSpeed;
	if(glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = BaseSpeed * 4;
	}
	if(glfwGetKey(Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		speed = BaseSpeed / 4;
	}
    if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS){
        Position += direction * deltaTime * speed;
    }
    if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS){
        Position -= direction * deltaTime * speed;
    }
    if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS){
        Position += right * deltaTime * speed;
    }
    if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS){
        Position -= right * deltaTime * speed;
    }
    if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS){
        Position += up * deltaTime * speed;
    }
    if(glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS){
        Position -= up * deltaTime * speed;
    }

	Projection = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);

	View = glm::lookAt(
                       Position,
                       Position + direction,
                       up
                       );
}

glm::mat4 Camera::getProjectionMatrix() {
	return Projection;
}

glm::mat4 Camera::getViewMatrix() {
	return View;
}

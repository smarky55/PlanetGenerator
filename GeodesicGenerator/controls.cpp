#include "controls.h"

GLFWwindow* window;



glm::mat4 Projection;
glm::mat4 View;// = glm::lookAt(
                 //            glm::vec3(4,3,-3),
                   //          glm::vec3(0,0,0),
                     //        glm::vec3(0,1,0)
                       //      );

glm::mat4 getProjectionMatrix(){
    return Projection;
}

glm::mat4 getViewMatrix(){
    return View;
}


glm::vec3 position = glm::vec3(0.5,1,1);
float horizontalAngle = 3.14f;
float verticalAngle = -0.2;
float initialFOV = 45.0f;

float baseSpeed = 3.0f;
float mouseSpeed = 0.005f;

void computeMatrices(){
    window = glfwGetCurrentContext();
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();

    float deltaTime = float(lastTime - currentTime);

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
	glfwSetCursorPos(window, 1024 / 2, 786 / 2);

    horizontalAngle += mouseSpeed * float(1024/2 - xPos);
	horizontalAngle = std::fmod(horizontalAngle, 3.14f*2);
    verticalAngle += mouseSpeed * float(786/2 - yPos);
	verticalAngle = glm::clamp(verticalAngle, -1.57f, 1.57f);

    glm::vec3 direction = glm::vec3(
                                    cos(verticalAngle) * sin(horizontalAngle),
                                    sin(verticalAngle),
                                    cos(verticalAngle) * cos(horizontalAngle)
                                    );

    glm::vec3 right = glm::vec3(
                                sin(horizontalAngle - 3.14f/2.0f),
                                0,
                                cos(horizontalAngle - 3.14f/2.0f)
                                );
    glm::vec3 up = glm::cross(right, direction);
	float speed = baseSpeed;
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = baseSpeed * 4;
	}
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        position -= up * deltaTime * speed;
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        position += up * deltaTime * speed;
    }

    float FoV = initialFOV;

    Projection = glm::perspective(FoV, 4.0f/3.0f, 0.1f, 100.0f);

    View = glm::lookAt(
                       position,
                       position + direction,
                       up
                       );

	/*std::cout << direction.x << ' ' << direction.y << ' ' << direction.z << std::endl;
	std::cout << horizontalAngle << ' ' << verticalAngle << std::endl;*/
	//std::cout << position.x << ' ' << position.y << ' ' << position.z << std::endl;
    lastTime = currentTime;
}

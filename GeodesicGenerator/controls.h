#pragma once

#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();
void computeMatrices();
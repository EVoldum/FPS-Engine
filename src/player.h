#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void updatePlayer(GLFWwindow *window);
void handleShooting(GLFWwindow *window);
glm::vec3 getEyePosition();
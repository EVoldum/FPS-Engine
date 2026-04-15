#pragma once

#include <glm/glm.hpp>
#include <vector>

bool checkCollision(glm::vec3 point, glm::vec3 cubePos, float size = 1.0f);

bool rayIntersectsCube(glm::vec3 origin, glm::vec3 dir, glm::vec3 cubePos, float size);

int raycast(glm::vec3 origin, glm::vec3 direction,
            std::vector<glm::vec3> &cubes,
            std::vector<uint8_t> &active);
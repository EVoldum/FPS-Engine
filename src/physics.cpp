#include "physics.h"
#include <algorithm> // for std::swap
#include <iostream>

bool checkCollision(glm::vec3 point, glm::vec3 cubePos, float size)
{
    float half = size / 2.0f + 0.001f; // add small epsilon to prevent clipping issues

    return (
        point.x > cubePos.x - half && point.x < cubePos.x + half &&
        point.y > cubePos.y - half && point.y < cubePos.y + half &&
        point.z > cubePos.z - half && point.z < cubePos.z + half);
}

int raycast(glm::vec3 origin, glm::vec3 direction,
            std::vector<glm::vec3> &cubes,
            std::vector<bool> &active)
{
    direction = glm::normalize(direction);

    float maxDistance = 100.0f;
    float step = 0.05f; // smaller = more accurate

    for (float d = 0.0f; d < maxDistance; d += step)
    {
        glm::vec3 point = origin + direction * d;

        for (int i = 0; i < cubes.size(); i++)
        {
            if (!active[i])
                continue;

            if (checkCollision(point, cubes[i]))
            {
                return i;
            }
        }
    }

    return -1;
}
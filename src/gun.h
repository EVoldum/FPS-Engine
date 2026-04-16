#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Gun
{
    unsigned int VAO, VBO;
    int vertexCount;
};

Gun loadGun(const std::string &objPath);
void drawGun(unsigned int shaderProgram, const Gun &gun,
             unsigned int modelLoc, unsigned int viewLoc,
             unsigned int projLoc, unsigned int colorLoc);
void cleanupGun(Gun &gun);
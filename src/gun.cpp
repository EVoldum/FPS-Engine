#define TINYOBJLOADER_IMPLEMENTATION
#include "../libs/tiny_obj_loader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "gun.h"
#include "globals.h"
#include "player.h"

Gun loadGun(const std::string &objPath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str());

    if (!ok)
    {
        std::cout << "Failed to load gun OBJ: " << err << "\n";
        return {0, 0, 0};
    }

    // Flatten into interleaved [x,y,z, nx,ny,nz] to match your existing shader
    std::vector<float> verts;

    for (auto &shape : shapes)
    {
        for (auto &idx : shape.mesh.indices)
        {
            // Position
            verts.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            verts.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            verts.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

            // Normal (use (0,1,0) as fallback if OBJ has no normals)
            if (idx.normal_index >= 0)
            {
                verts.push_back(attrib.normals[3 * idx.normal_index + 0]);
                verts.push_back(attrib.normals[3 * idx.normal_index + 1]);
                verts.push_back(attrib.normals[3 * idx.normal_index + 2]);
            }
            else
            {
                verts.push_back(0.0f);
                verts.push_back(1.0f);
                verts.push_back(0.0f);
            }
        }
    }

    Gun gun;
    gun.vertexCount = verts.size() / 6;

    glGenVertexArrays(1, &gun.VAO);
    glGenBuffers(1, &gun.VBO);

    glBindVertexArray(gun.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, gun.VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // Position — location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal — location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    std::cout << "Gun loaded: " << gun.vertexCount << " vertices\n";
    return gun;
}

void drawGun(unsigned int shaderProgram, const Gun &gun,
             unsigned int modelLoc, unsigned int viewLoc,
             unsigned int projLoc, unsigned int colorLoc)
{
    float speed = glm::length(velocity);
    if (speed > 0.1f)
        gunBobTime += deltaTime * 8.0f;

    float bobX = sin(gunBobTime) * 0.012f;
    float bobY = sin(gunBobTime * 2.0f) * 0.008f;

    glm::vec3 eyePos = getEyePosition();
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));

    glm::vec3 gunOrigin = eyePos + right * (0.25f + bobX) + up * (-0.22f + bobY) + cameraFront * 0.5f;

    gunOrigin -= cameraFront * (recoilOffset * 0.04f);

    // Orientation: align gun's axes with camera axes
    glm::mat4 rot(1.0f);
    rot[0] = glm::vec4(right, 0.0f);
    rot[1] = glm::vec4(up, 0.0f);
    rot[2] = glm::vec4(-cameraFront, 0.0f);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), gunOrigin - glm::vec3(0.0f, 1.25f, 0.0f));
    model = model * rot;
    model = glm::scale(model, glm::vec3(0.01f));
    model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view = glm::lookAt(eyePos, eyePos + cameraFront, cameraUp);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniform3f(colorLoc, 0.3f, 0.3f, 0.32f);

    glBindVertexArray(gun.VAO);
    glDrawArrays(GL_TRIANGLES, 0, gun.vertexCount);
    glBindVertexArray(0);
}

void cleanupGun(Gun &gun)
{
    glDeleteVertexArrays(1, &gun.VAO);
    glDeleteBuffers(1, &gun.VBO);
}
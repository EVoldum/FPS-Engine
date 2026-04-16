#include "render.h"
#include <glad/glad.h>
#include "globals.h"
#include "player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void drawCrosshair()
{
    float size = 0.02f;

    float crosshairVertices[] = {
        -size, 0.0f, 0.0f,
        size, 0.0f, 0.0f,

        0.0f, -size, 0.0f,
        0.0f, size, 0.0f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 4);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void drawGun(unsigned int shaderProgram, unsigned int VAO,
             unsigned int modelLoc, unsigned int viewLoc,
             unsigned int projLoc, unsigned int colorLoc)
{
    // Update bob
    float speed = glm::length(velocity);
    if (speed > 0.1f)
        gunBobTime += deltaTime * 8.0f;

    float bobX = sin(gunBobTime) * 0.012f;
    float bobY = sin(gunBobTime * 2.0f) * 0.008f;

    glm::vec3 eyePos = getEyePosition();

    // Build a proper view basis from cameraFront
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));

    // Gun anchor point in world space (right, down, forward from eye)
    glm::vec3 gunOrigin = eyePos + right * (0.25f + bobX) + up * (-0.22f + bobY) + cameraFront * 0.5f;

    // Recoil: push gun back along cameraFront
    gunOrigin -= cameraFront * (recoilOffset * 0.04f);

    // View/projection: same as world but we clear depth first
    glm::mat4 view = glm::lookAt(eyePos, eyePos + cameraFront, cameraUp);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    // Rotation matrix to align gun with camera facing direction
    // Build a model matrix whose axes ARE the camera axes
    glm::mat4 rot = glm::mat4(1.0f);
    rot[0] = glm::vec4(right, 0.0f);
    rot[1] = glm::vec4(up, 0.0f);
    rot[2] = glm::vec4(-cameraFront, 0.0f); // OpenGL -Z forward

    glBindVertexArray(VAO);

    // --- GUN BODY ---
    glUniform3f(colorLoc, 0.25f, 0.25f, 0.28f); // dark grey

    glm::mat4 body = glm::translate(glm::mat4(1.0f), gunOrigin);
    body = body * rot;
    body = glm::scale(body, glm::vec3(0.08f, 0.08f, 0.28f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(body));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- GUN BARREL (thinner, offset forward and up slightly) ---
    glUniform3f(colorLoc, 0.15f, 0.15f, 0.18f); // slightly darker

    glm::vec3 barrelOrigin = gunOrigin + up * 0.035f + cameraFront * 0.22f;

    glm::mat4 barrel = glm::translate(glm::mat4(1.0f), barrelOrigin);
    barrel = barrel * rot;
    barrel = glm::scale(barrel, glm::vec3(0.04f, 0.04f, 0.2f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(barrel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
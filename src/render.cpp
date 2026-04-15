#include "render.h"
#include <glad/glad.h>

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
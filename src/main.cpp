#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "globals.h"
#include "camera.h"
#include "input.h"
#include "physics.h"
#include "render.h"
#include "player.h"
#include "audio.h"
#include "gun.h"

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aNormal;\n"
                                 "out vec3 Normal;\n"
                                 "out vec3 FragPos;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main() {\n"
                                 "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
                                 "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
                                 "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 Normal;\n"
                                   "in vec3 FragPos;\n"
                                   "uniform vec3 color;\n"
                                   "uniform vec3 lightDir;\n"
                                   "void main() {\n"
                                   "   float diff = max(dot(normalize(Normal), -lightDir), 0.2);\n"
                                   "   vec3 result = diff * color;\n"
                                   "   FragColor = vec4(result, 1.0);\n"
                                   "}\n\0";

// Resize window properly
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW
    glfwInit();

    // Tell OpenGL version (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 600, "My First 3D Engine", NULL, NULL);

    glfwMakeContextCurrent(window); // Make the window's context current

    glfwSwapInterval(1); // Enable vsync

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        // positions          // normals

        // back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        // front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        // left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        // right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        // bottom
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        // top
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");

    // SEND TO SHADER
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f);

    // Floor (grid)
    for (int x = -5; x <= 5; x++)
    {
        for (int z = -5; z <= 5; z++)
        {
            cubePositions.push_back(glm::vec3(x, -1.0f, z));
        }
    }

    // Walls
    for (int i = -5; i <= 5; i++)
    {
        if (i != -5 && i != 5)
        {
            cubePositions.push_back(glm::vec3(i, 0.0f, -5));
            cubePositions.push_back(glm::vec3(i, 0.0f, 5));
        }

        cubePositions.push_back(glm::vec3(-5, 0.0f, i));
        cubePositions.push_back(glm::vec3(5, 0.0f, i));
    }

    cubeActive.resize(cubePositions.size(), true);
    hitTimer.resize(cubePositions.size(), 0.0f);

    initAudio();

    Gun gun = loadGun("assets/ak47/Ak-47.obj");
    std::cout << "Gun vertex count: " << gun.vertexCount << "\n";

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // ESC - Exit game
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        // TIME
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // PLAYER SYSTEM
        updatePlayer(window);

        // CAMERA SMOOTHING
        float t = 1.0f - exp(-mouseSmoothness * deltaTime);

        smoothYaw += (yaw - smoothYaw) * t;
        smoothPitch += (pitch - smoothPitch) * t;

        updateCameraDirection();

        // SHOOTING
        handleShooting(window);

        // RECOIL RECOVERY
        if (recoilOffset > 0.0f)
        {
            recoilOffset -= recoilRecovery * deltaTime;

            if (recoilOffset < 0.0f)
                recoilOffset = 0.0f;
        }

        // GET CAMERA POSITION (ONCE)
        glm::vec3 eyePos = getEyePosition();

        // CLEAR
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // SHADER
        glUseProgram(shaderProgram);

        glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -0.5f));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, &lightDir[0]);

        glUniform3f(colorLoc, 1.0f, 0.5f, 0.2f);

        // VIEW / PROJECTION
        glm::mat4 view = glm::lookAt(eyePos, eyePos + cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            800.0f / 600.0f,
            0.1f,
            100.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // DRAW WORLD
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            if (!cubeActive[i])
                continue;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // GUN VIEWMODEL
        glClear(GL_DEPTH_BUFFER_BIT);
        drawGun(shaderProgram, gun, modelLoc, viewLoc, projLoc, colorLoc);

        // CROSSHAIR
        glUniform3f(colorLoc, 0.0f, 0.5f, 1.0f);
        glDisable(GL_DEPTH_TEST);

        glm::mat4 identity = glm::mat4(1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(identity));

        drawCrosshair();

        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanupGun(gun);
    cleanupAudio();
    glfwTerminate();
    return 0;
}
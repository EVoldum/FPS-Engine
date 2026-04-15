#include "player.h"
#include "globals.h"
#include "physics.h"
#include "audio.h"
#include <iostream>

// movement + physics
void updatePlayer(GLFWwindow *window)
{
    float baseSpeed = 8.0f;
    float sprintMultiplier = 2.0f;

    float cameraSpeed = baseSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= sprintMultiplier;

    glm::vec3 newPos = cameraPos;

    glm::vec3 flatFront = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 right = glm::normalize(glm::cross(flatFront, cameraUp));

    glm::vec3 wishDir(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        wishDir += flatFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        wishDir -= flatFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        wishDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        wishDir += right;

    if (glm::length(wishDir) > 0.0f)
        wishDir = glm::normalize(wishDir);

    float control = onGround ? 1.0f : 0.3f;
    velocity += wishDir * acceleration * control * deltaTime;

    float maxSpeed = baseSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        maxSpeed *= sprintMultiplier;

    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;

    float speed = glm::length(velocity);

    if (speed > 0.0f)
    {
        float drop = speed * friction * deltaTime;
        float newSpeed = std::max(0.0f, speed - drop);
        velocity *= newSpeed / speed;
    }

    newPos += velocity * deltaTime;

    // gravity
    yVelocity += gravity * deltaTime;
    newPos.y += yVelocity * deltaTime;

    onGround = false;

    if (newPos.y <= 0.001f)
    {
        newPos.y = 0.0f;
        yVelocity = 0.0f;
        onGround = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
    {
        yVelocity = jumpForce;
    }

    // collision
    bool collided = false;

    for (unsigned int i = 0; i < cubePositions.size(); i++)
    {
        if (!cubeActive[i])
            continue;

        glm::vec3 headPos = newPos + glm::vec3(0.0f, playerHeight, 0.0f);

        if (checkCollision(newPos, cubePositions[i]) ||
            checkCollision(headPos, cubePositions[i]))
        {
            collided = true;
            break;
        }
    }

    if (!collided)
        cameraPos = newPos;
}

// shooting
void handleShooting(GLFWwindow *window)
{
    bool currentMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (currentMouseState)
    {
        float currentTime = glfwGetTime();

        if (currentTime - lastShotTime >= fireCooldown)
        {
            playSound("assets/shot.mp3");

            recoilOffset += recoilAmount;
            lastShotTime = currentTime;

            glm::vec3 eyePos = getEyePosition();

            float spread = 0.008f;

            glm::vec3 shotDir = cameraFront;
            shotDir.x += ((rand() % 100) / 100.0f - 0.5f) * spread;
            shotDir.y += ((rand() % 100) / 100.0f - 0.5f) * spread;

            shotDir = glm::normalize(shotDir);

            int hit = raycast(eyePos, cameraFront, cubePositions, cubeActive);

            if (hit != -1 && cubeActive[hit])
            {
                cubeActive[hit] = false;
                hitTimer[hit] = 0.1f;
                std::cout << "Destroyed cube: " << hit << "\n";
            }

            if (hit != -1)
                std::cout << "Hit cube: " << hit << "\n";
            else
                std::cout << "Missed\n";
        }
    }
}

// single source of truth
glm::vec3 getEyePosition()
{
    glm::vec3 eyePos = cameraPos + glm::vec3(0.0f, playerHeight, 0.0f);

    float speed = glm::length(velocity);
    float bob = 0.0f;

    if (speed > 0.1f)
        bob = sin(glfwGetTime() * 8.0f) * 0.015f;

    eyePos.y += bob;

    return eyePos;
}
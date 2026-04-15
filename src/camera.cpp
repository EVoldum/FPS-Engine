#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void updateCameraDirection()
{
    float finalPitch = smoothPitch + recoilOffset;
    float finalYaw = smoothYaw;

    if (finalPitch > 89.0f)
        finalPitch = 89.0f;
    if (finalPitch < -89.0f)
        finalPitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(finalYaw)) * cos(glm::radians(finalPitch));
    direction.y = sin(glm::radians(finalPitch));
    direction.z = sin(glm::radians(finalYaw)) * cos(glm::radians(finalPitch));

    cameraFront = glm::normalize(direction);
}

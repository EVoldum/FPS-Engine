#include "globals.h"

// CAMERA
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float smoothYaw = yaw;
float smoothPitch = pitch;
float mouseSmoothness = 25.0f;

// TIME
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// MOVEMENT
glm::vec3 velocity(0.0f);
float acceleration = 40.0f;
float friction = 8.0f;

float yVelocity = 0.0f;
bool onGround = false;

// JUMPING
const float gravity = -9.8f;
const float jumpForce = 5.0f;

// PLAYER
const float playerHeight = 1.8f;

// SHOOTING
float fireCooldown = 0.2f;
float lastShotTime = 0.0f;

// RECOIL
float recoilOffset = 0.0f;
float recoilRecovery = 12.0f;
float recoilAmount = 1.5f;

// WORLD
std::vector<glm::vec3> cubePositions;
std::vector<uint8_t> cubeActive;

std::vector<float> hitTimer;

float gunBobTime = 0.0f;
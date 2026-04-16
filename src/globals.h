#pragma once

#include <glm/glm.hpp>
#include <vector>

// CAMERA
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern float yaw, pitch;
extern float smoothYaw, smoothPitch;
extern float mouseSmoothness;

// TIME
extern float deltaTime;
extern float lastFrame;

// MOVEMENT
extern glm::vec3 velocity;
extern float acceleration;
extern float friction;

extern float yVelocity;
extern bool onGround;

// JUMPING
extern const float gravity;
extern const float jumpForce;

// PLAYER
extern const float playerHeight;

// SHOOTING
extern float fireCooldown;
extern float lastShotTime;

// RECOIL
extern float recoilOffset;
extern float recoilRecovery;
extern float recoilAmount;

// WORLD
extern std::vector<glm::vec3> cubePositions;
extern std::vector<uint8_t> cubeActive;

// Hit feedback
extern std::vector<float> hitTimer;

// GUN
extern float gunBobTime;
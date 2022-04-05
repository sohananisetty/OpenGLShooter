#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include<iostream>

#include"GameObject.h"
#include "Enemy.h"
#include "Settings.h"

//enum class Player_Movement {
//    FORWARD,
//    BACKWARD,
//    LEFT,
//    RIGHT,
//    JUMP,
//};

// Default values
const float GRAVITY = -10.0f;


class Player:public GameObject
{
public:
  
    float PlayerMouseSensitivity, PlayerTurnSpeed , PlayerJumpSpeed, currentUpSpeed;


    // constructor with vectors
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float size = 1.0f);
    
    glm::mat4 GetModelMatrix();

    glm::mat4 GetModelMatrix(float yawOffset);


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(bool* keys, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset);

    void updatePlayerVectors();

    bool CheckCollision(Enemy& obj);

    bool CheckCollision(GameObject& obj);

};


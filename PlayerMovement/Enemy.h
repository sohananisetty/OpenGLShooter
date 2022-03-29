#ifndef ENEMY_H
#define ENEMY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"

#include <vector>
#include<iostream>


class Enemy :public GameObject
{
public:

    //glm::vec3 enemyStartPosition;
    glm::vec3 targetPosition;


    Enemy(glm::vec3 spawnPosition, float size, float speed);

    glm::vec3 Move(float dt, glm::vec3 targetPos);

    glm::mat4 GetModelMatrix(glm::vec3  enemyPosition , float yawOffset);
};

#endif
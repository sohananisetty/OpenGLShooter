#ifndef BULLET_H
#define BULLET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.h"
#include "Enemy.h"


#include <vector>
#include<iostream>

//const float BULLET_DIE_DISTANCE = 10.0f;

class Bullet : public GameObject
{
public:
    // ball state	
    glm::vec3  bulletStartPosition;

    // constructor(s)
 
   // Bullet();
    Bullet(glm::vec3 position, float radius, float speed, float yaw);

    glm::vec3 Move(float dt);

    glm::mat4 GetModelMatrix(glm::vec3  bulletPosition );

    bool CheckCollision(Enemy& obj);

};

#endif
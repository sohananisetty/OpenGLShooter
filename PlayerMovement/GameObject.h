#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cmath>

#include "Model.h"
#include "Settings.h"

enum class Object_Movement {
    STILL,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
};

class GameObject
{
public:
    Model model;
    Settings settings;

    // object state
    glm::vec3 objectPosition;
    glm::vec3 objectFront;
    glm::vec3 objectRight;
    glm::vec3 objectUp;
    glm::vec3 worldUp;
    std::vector<float> boundingBox; // x max,min y max,min z max, min
//    GameObject* boundingBoxModel;
   
    Object_Movement currentMovement = Object_Movement::STILL;
    Object_Movement lastMovement = Object_Movement::STILL;


    float yaw;
    float pitch;
    float objectSpeed;
    glm::vec3 objectSize;
    glm::vec3 objectColor;
    bool  destroyed;

    unsigned int VBO, VAO, EBO;

    // constructor(s)
    GameObject();
    GameObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f),glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f, float speed = 1.0f , bool dest = false);
    //GameObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f, float speed = 1.0f, bool dest = false);

    void LinkMesh(Model model);

    void Draw(Shader& shader);

    glm::mat4 GetModelMatrix();
    glm::mat4 GetModelMatrix(glm::vec3 scale , float yawOffset);

    bool CheckCollision(GameObject& obj);


protected:

    void calcObjectVectors(float yaw, float pitch);
    void calcObjectVectors(glm::vec3 front);




};

#endif
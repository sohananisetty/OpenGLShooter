#include "GameObject.h"


GameObject::GameObject(): objectPosition(0.0f, 0.0f , 0.0f), worldUp(0.0f,1.0f,0.0f), objectSize(glm::vec3(0.0f)), objectColor(glm::vec3(1.0f)), objectSpeed(0.0f), yaw(0.0f), pitch(0.0f), destroyed(false)
{
    boundingBox = { 0.0f, 0.0f, 0.0f,0.0f,0.0f,0.0f };
    calcObjectVectors(yaw, pitch);
}

GameObject::GameObject(glm::vec3 position, glm::vec3 size, glm::vec3 worldUp, float yaw, float pitch,  float speed, bool dest)
    : objectPosition(position), worldUp(worldUp), objectSize(size), objectColor(glm::vec3(1.0f)),objectSpeed( speed), yaw(yaw), pitch(pitch), destroyed(dest)
{
    boundingBox = { 0.0f, 0.0f, 0.0f,0.0f,0.0f,0.0f };
    calcObjectVectors(this->yaw, this->pitch);


}


void GameObject::LinkMesh(Model model)
{
    this->model = model;
    float k = 0;
    for (int i = 0; i < model.boundingBox.size(); i++ , k+=0.5) {
        this->boundingBox[i] = model.boundingBox[i] * objectSize[(int)floor(k)];
    }


}

void GameObject::Draw(Shader& shader)
{
    
    this->model.Draw(shader);
}

glm::mat4 GameObject::GetModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->objectPosition);
    model = glm::rotate(model, glm::radians(this->pitch), this->objectRight);
    model = glm::rotate(model, glm::radians(this->yaw), this->objectUp);
    model = glm::scale(model, glm::vec3(this->objectSize));

    return model;
}

glm::mat4 GameObject::GetModelMatrix(glm::vec3 scale , float yawOffset = 0.0f)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->objectPosition);
    model = glm::rotate(model, glm::radians(this->yaw + yawOffset), this->objectUp);
    model = glm::scale(model, scale);

    return model;
}


void GameObject::calcObjectVectors(float yaw, float pitch)
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->objectFront = glm::normalize(front);
    this->objectRight = glm::normalize(glm::cross(this->objectFront, worldUp));
    this->objectUp = glm::normalize(glm::cross(this->objectRight, this->objectFront));

}

void GameObject::calcObjectVectors(glm::vec3 front)
{
    this->objectFront = glm::normalize(front);
    this->objectRight = glm::normalize(glm::cross(this->objectFront, worldUp));
    this->objectUp = glm::normalize(glm::cross(this->objectRight, this->objectFront));

    this->pitch = glm::degrees(asin(this->objectFront.y));
    float inv = front.x / cos(glm::radians(pitch));
    this->yaw = this->objectFront.z >0.0f?360.0 - glm::degrees(acos(inv)) : glm::degrees(acos(inv));
//    std::cout << "front_y " << front.x / cos(glm::radians(pitch)) <<" yaw " << this->yaw << "\n";

}

bool GameObject::CheckCollision(GameObject& obj) 
{
    // collision x-axis?
    bool collisionX = this->objectPosition.x + this->boundingBox[0] >= obj.objectPosition.x - glm::abs(obj.boundingBox[1]) &&
        this->objectPosition.x - glm::abs(this->boundingBox[1]) <= obj.objectPosition.x + glm::abs(obj.boundingBox[0]);
    // collision y-axis?
    bool collisionY = this->objectPosition.z + this->boundingBox[4] >= obj.objectPosition.z - glm::abs(obj.boundingBox[5]) &&
        this->objectPosition.z - glm::abs(this->boundingBox[4]) <= obj.objectPosition.z + glm::abs(obj.boundingBox[5]);
    // collision only if on both axes
    return collisionX && collisionY;
}
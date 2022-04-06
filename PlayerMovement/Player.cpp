#include "Player.h"


Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float size) :
    GameObject(position,glm::vec3(size),up,yaw,0.0f,settings.PLAYER_SPEED,false) , PlayerMouseSensitivity(settings.PLAYER_SENSITIVITY), PlayerTurnSpeed(settings.PLAYER_TURNSPEED), currentUpSpeed(0.0f), PlayerJumpSpeed(settings.PLAYER_JUMP_SPEED)
{
    this->objectSpeed = settings.PLAYER_SPEED;
    //updatePlayerVectors();
}

glm::mat4 Player::GetModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, objectPosition);
    model = glm::rotate(model, glm::radians(yaw), objectUp);
    model = glm::scale(model, glm::vec3(objectSize));

    return model;

}

glm::mat4 Player::GetModelMatrix(float yawOffset)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->objectPosition);
    model = glm::rotate(model, glm::radians(this->yaw + yawOffset), this->objectUp);
    model = glm::scale(model, glm::vec3(this->objectSize));

    return model;
}


void Player::ProcessKeyboard(bool* keys, float deltaTime)
{
    float distance = this->objectSpeed * deltaTime;
    currentUpSpeed += GRAVITY * deltaTime;
    glm::vec3 currentPos = this->objectPosition;

    objectPosition.y += currentUpSpeed * deltaTime;

    if (objectPosition.y < 0.0f) {
        currentUpSpeed = 0.0f;
        objectPosition.y = 0.0f;
    }

    this->lastMovement = currentMovement;

    if (keys[GLFW_KEY_W]) {
       // std::cout << "Player speed: " << this->objectSpeed << "\n";
        objectPosition += objectFront * distance;
        this->currentMovement = Object_Movement::FORWARD;

    }

    if (keys[GLFW_KEY_S]) {

        objectPosition -= objectFront * distance;
        this->currentMovement = Object_Movement::BACKWARD;
    }

    if (keys[GLFW_KEY_A]) {

        objectPosition -= objectRight * distance;
        this->currentMovement = Object_Movement::LEFT;

     
    }

    if (keys[GLFW_KEY_D]) {

        objectPosition += objectRight * distance;
        this->currentMovement = Object_Movement::RIGHT;

    }
    if (keys[GLFW_KEY_SPACE]) {


        objectPosition += objectUp * PlayerJumpSpeed * deltaTime;
        this->currentMovement = Object_Movement::JUMP;

    }

    else if(!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_D] && !keys[GLFW_KEY_SPACE])
    {

        this->currentMovement = Object_Movement::STILL;

    }



}

void Player::ProcessMouseMovement(float xoffset)
{
    xoffset *= PlayerMouseSensitivity;

    yaw -= xoffset;
    if (yaw > 360.0f)
        yaw = yaw - 360.0f;

    if (yaw <0.0f)
        yaw = yaw + 360.0f;


    //std::cout << Yaw <<" "<<xoffset << "\n";

    // update Front, Right and Up Vectors using the updated Euler angles
    updatePlayerVectors();
}


void Player::updatePlayerVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));


    objectFront = glm::normalize(front);
    objectRight = glm::normalize(glm::cross(objectFront, worldUp));
    objectUp = glm::normalize(glm::cross(objectRight, objectFront));

   /* std::cout << "vectors\n";
    std::cout << playerFront.x<<" " << playerFront.y << " "<< playerFront.z<<"\n";
    std::cout << playerRight.x << " " << playerRight.y << " " << playerRight.z << "\n";
    std::cout << playerUp.x << " " << playerUp.y << " " << playerUp.z << "\n";*/

    
}



bool Player::CheckCollision(Enemy& obj)
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

bool Player::CheckCollision(GameObject& obj)
{
    bool x1 = this->objectPosition.x + this->boundingBox[0] >= obj.objectPosition.x - glm::abs(obj.boundingBox[1]);
    bool x2 = this->objectPosition.x - glm::abs(this->boundingBox[1]) <= obj.objectPosition.x + glm::abs(obj.boundingBox[0]);

    bool y1 = this->objectPosition.z + this->boundingBox[4] >= obj.objectPosition.z - glm::abs(obj.boundingBox[5]);
    bool y2 = this->objectPosition.z - glm::abs(this->boundingBox[4]) <= obj.objectPosition.z + glm::abs(obj.boundingBox[5]);
    // collision x-axis?
    bool collisionX = x1 && x2;
    // collision y-axis?
    bool collisionY = y1 && y2;


    // collision only if on both axes
    return collisionX && collisionY;
}
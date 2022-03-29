#include "Bullet.h"


//Bullet::Bullet():GameObject(), bulletStartPosition(objectPosition)
//{
//}
//GameObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f, float size = 1.0f, float speed = 1.0f, bool dest = false);
//
Bullet::Bullet(glm::vec3 position, float radius, float speed, float yaw)
	:GameObject(position, radius, glm::vec3(0.0f,1.0f,0.0f), yaw, 0.0f, speed,false)
{
	this->bulletStartPosition = position;
}

glm::vec3 Bullet::Move(float dt)
{
	if (!destroyed){
		this->objectPosition += objectFront * dt * this->objectSpeed;

		float distance = glm::length(this->bulletStartPosition - this->objectPosition);

		if (distance >= BULLET_DIE_DISTANCE)
		{
			this->destroyed = true;
		}
		
	}
	
	return this->objectPosition;
}


glm::mat4 Bullet::GetModelMatrix(glm::vec3  bulletCurrentPosition)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, bulletCurrentPosition);
	model = glm::scale(model, glm::vec3(this->objectSize));

	return model;
}


bool Bullet::CheckCollision(Enemy& obj)
{
	// collision x-axis?
	bool collisionX = this->objectPosition.x + this->boundingBox[0] >= obj.objectPosition.x - glm::abs(obj.boundingBox[1]) &&
		this->objectPosition.x - glm::abs(this->boundingBox[1]) <= obj.objectPosition.x + glm::abs(obj.boundingBox[0]);
	// collision y-axis?
	bool collisionY = this->objectPosition.z + this->boundingBox[2] >= obj.objectPosition.z - glm::abs(obj.boundingBox[3]) &&
		this->objectPosition.z - glm::abs(this->boundingBox[3]) <= obj.objectPosition.z + glm::abs(obj.boundingBox[2]);
	// collision only if on both axes
	return collisionX && collisionY;
}
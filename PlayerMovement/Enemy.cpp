#include "Enemy.h"


Enemy::Enemy(glm::vec3 position, float size, float speed)
	:GameObject(position, glm::vec3(size), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, speed, false)
{
}

glm::vec3 Enemy::Move(float dt, glm::vec3 targetPos , Terrain terrain)
{
	if (!destroyed) {
		this->objectFront = glm::normalize(glm::vec3(targetPos.x - this->objectPosition.x,0.0f, targetPos.z - this->objectPosition.z));
		this->objectPosition += objectFront * dt * this->objectSpeed;
		this->objectPosition.y = terrain.getTerrainHeight(this->objectPosition.x, this->objectPosition.z);
		calcObjectVectors(this->objectFront); 
	}

	return this->objectPosition;
}


glm::mat4 Enemy::GetModelMatrix(glm::vec3  enemyCurrentPosition , float yawOffset)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, enemyCurrentPosition);
	model = glm::rotate(model, glm::radians(this->yaw+ yawOffset), this->objectUp);
	model = glm::scale(model, glm::vec3(this->objectSize));

	return model;
}
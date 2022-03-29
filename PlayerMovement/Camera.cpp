#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

glm::mat4 Camera::GetViewMatrix(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp)
{
    return glm::lookAt(cameraPosition, cameraTarget, cameraUp);

}
//
//void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
//{
//    float velocity = MovementSpeed * deltaTime;
//    if (direction == Camera_Movement::FORWARD)
//        cameraPosition += cameraFront * velocity;
//    if (direction == Camera_Movement::BACKWARD)
//        cameraPosition -= cameraFront * velocity;
//    if (direction == Camera_Movement::LEFT)
//        cameraPosition -= cameraRight * velocity;
//    if (direction == Camera_Movement::RIGHT)
//        cameraPosition += cameraRight * velocity;
//}
//
//void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
//{
//    xoffset *= MouseSensitivity;
//    yoffset *= MouseSensitivity;
//
//    Yaw += xoffset;
//    Pitch += yoffset;
//
//    // make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (constrainPitch)
//    {
//        if (Pitch > 89.0f)
//            Pitch = 89.0f;
//        if (Pitch < -89.0f)
//            Pitch = -89.0f;
//    }
//
//    // update Front, Right and Up Vectors using the updated Euler angles
//    updateCameraVectors();
//}


void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    cameraRight = glm::normalize(glm::cross(cameraFront, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
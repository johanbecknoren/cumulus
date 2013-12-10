#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	pos = glm::vec3(0, 0, 0);
	dir = glm::vec3(0, 0, 1.f);
	up = glm::vec3(0, 1.f, 0.f);
	view = glm::lookAt(pos, pos + dir, up);
}
glm::mat4 Camera::getProjection() {
	return projection;
}

glm::mat4 Camera::getView() {
	return view;
}

void Camera::translate(glm::vec3 dir) {
	pos += dir;
}

void Camera::rotate(float ammount, glm::vec3 axis) {
	dir = glm::rotate(dir, ammount, axis);
}

void Camera::update() {
	view = glm::lookAt(pos, pos + dir, up);
}

void Camera::applyMovement(MovementType movement)
{
    switch (movement)
    {
        case FORWARD:
            pos += dir;
        break;
        case BACKWARD:
            pos -= dir;
        break;
        case STRAFE_L:
            pos += glm::cross(dir, up);
        break;
        case STRAFE_R:
            pos -= glm::cross(dir, up);
        break;
    }
}
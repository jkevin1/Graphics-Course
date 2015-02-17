#include "Camera.hpp"
#include "Vec.inl"

Camera::Camera() : position() {
	yaw = pitch = 0.0f;
}

Camera::Camera(Vec<3> pos, float yaw, float pitch) : position(pos) {
	this->yaw = yaw;
	this->pitch = pitch;
}

void Camera::translate(const Vec<3>& displacement) {
	position = position + displacement;
}

void Camera::move(const Vec<3>& displacement) {
	float cos = cosf(yaw);
	float sin = sinf(yaw);
	position.x += MOVESPEED * (-displacement.x*sin - displacement.y*cos);
	position.y += MOVESPEED * (-displacement.x*cos + displacement.y*sin);
	position.z += MOVESPEED * displacement.z;
}

void Camera::look(float dYaw, float dPitch) {
	yaw += X_SENSITIVITY * dYaw;
	pitch += Y_SENSITIVITY * dPitch;
	if (pitch < -F_PI/2) pitch = -F_PI/2;
	if (pitch > F_PI/2) pitch = F_PI/2;
}
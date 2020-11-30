#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
}

void Camera::Update(ULONGLONG dt)
{
	x += vx * dt;
}

Camera::~Camera()
{
}

#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
}

void Camera::Update(DWORD dt)
{
	x += vx * dt;
}

Camera::~Camera()
{
}

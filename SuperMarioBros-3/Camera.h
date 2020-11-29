#pragma once
#include "define.h"
#include "Mario.h"

class Camera
{
	float x, y, vx, vy;

public:
	CMario* player;

	Camera(CMario* player);

	void Update(DWORD dt);

	void SetCamPos(float x, float y) { this->x = x; this->y = y; }
	float GetCamPosX() { return x; }
	float GetCamPosY() { return y; }
	float GetCamSpeedX() { return vx; }
	float GetCamSpeedY() { return vy; }

	~Camera();
};


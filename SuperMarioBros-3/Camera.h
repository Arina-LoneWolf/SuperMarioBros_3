#pragma once
#include "define.h"
#include "Mario.h"

class Camera
{
	float camX, camY, camSpeedX = 0, camSpeedY = 0;

public:
	CMario* player;

	bool goToHiddenArea = false;
	bool backFromHiddenArea = false;
	bool inHiddenArea = false;
	bool goToBackyard = false;

	Camera(CMario* player);

	void Update(ULONGLONG dt);

	void SetCamPos(float x, float y) { this->camX = x; this->camY = y; }
	float GetCamPosX() { return camX; }
	float GetCamPosY() { return camY; }
	float GetCamSpeedX() { return camSpeedX; }
	float GetCamSpeedY() { return camSpeedY; }

	~Camera();
};


#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
	//camX = playerX - (SCREEN_WIDTH / 4);
	camY = 200;
	CGame::GetInstance()->SetCamPos(camX, camY);
}

void Camera::Update(ULONGLONG dt)
{
	//DebugOut(L"CAM y: %f\n", camY);
	//DebugOut(L"isWaggingTail = %d\n", player->isWaggingTail);
	//DebugOut(L"mario vy: %f\n", player->vy);
	float left, top, right, bottom;
	
	player->GetBoundingBox(left, top, right, bottom);

	if (player->isOnGround)
	{
		camSpeedY = 0;
		goto SET_CAM;
	}

	if (player->canFly && !player->isOnGround)
	{
		if (player->vy < 0 && top + 13 < camY + SCREEN_HEIGHT / 6)
		{
			camSpeedY = player->vy;
		}
		else if (player->vy > 0 && top + 7 > camY + SCREEN_HEIGHT / 4)
		{
			camSpeedY = player->vy;
		}
		else
			camSpeedY = 0;
	}
	else
	{
		if (player->vy < 0 && top + 13 < round(camY + SCREEN_HEIGHT / 6) && camY <= 200)
		{
			camSpeedY = player->vy;
		}
		else if (player->vy > 0 && top + 7 > round(camY + SCREEN_HEIGHT / 4 - 20) && camY <= 200)
			camSpeedY = player->vy;
		else
			camSpeedY = 0;
	}

SET_CAM:

	camY += camSpeedY * dt;
	if (camY <= 0 || camY > 200)
		return;

	CGame::GetInstance()->SetCamPosY(camY);
}

Camera::~Camera()
{
}

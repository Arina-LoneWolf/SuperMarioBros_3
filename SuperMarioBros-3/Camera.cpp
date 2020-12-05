#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
	//camX = playerX - (SCREEN_WIDTH / 4);
	camY = CAMERA_INITIAL_Y;
	CGame::GetInstance()->SetCamPos(camX, camY);
}

void Camera::Update(ULONGLONG dt)
{
	//DebugOut(L"CAM y: %f\n", camY);
	//DebugOut(L"isWaggingTail = %d\n", player->isWaggingTail);
	//DebugOut(L"mario vy: %f\n", player->vy);
	float playerTop = player->GetTop();

	if (player->isOnGround)
	{
		camSpeedY = 0;
		goto SET_CAM;
	}

	if (player->canFly && !player->isOnGround)
	{
		if (player->vy < 0 && playerTop + 20 < camY + 5 * SCREEN_HEIGHT / 42 - 11)
		{
			camSpeedY = player->vy;
		}
		else if (player->vy > 0 && playerTop + 7 > camY + 5 * SCREEN_HEIGHT / 28 - 17)
		{
			camSpeedY = player->vy;
		}
		else
			camSpeedY = 0;
	}
	else
	{
		if (player->vy < 0 && playerTop + 20 < round(camY + 5 * SCREEN_HEIGHT / 42 - 11) && camY <= CAMERA_INITIAL_Y)
		{
			camSpeedY = player->vy;
		}
		else if (player->vy > 0 && playerTop + 7 > round(camY + 5 * SCREEN_HEIGHT / 28 - 17 - 20) && camY <= CAMERA_INITIAL_Y)
			camSpeedY = player->vy;
		else
			camSpeedY = 0;
	}

SET_CAM:

	camY += camSpeedY * dt;
	if (camY <= 0 || camY > CAMERA_INITIAL_Y)
		return;

	CGame::GetInstance()->SetCamPosY(camY);
}

Camera::~Camera()
{
}

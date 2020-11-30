#include "Camera.h"

Camera::Camera(CMario* player, float playerX)
{
	this->player = player;
	camX = playerX - (SCREEN_WIDTH / 4);
	camY = 200;
	CGame::GetInstance()->SetCamPos(camX, camY);
}

void Camera::Update(ULONGLONG dt)
{
	//camX = CGame::GetInstance()->GetCamPosX();
	//camY = CGame::GetInstance()->GetCamPosY();
	
	DebugOut(L"MARIO speed y: %f\n", player->vy);
	DebugOut(L"CAM speed y: %f\n", camSpeedY);

	float left, top, right, bottom;
	
	player->GetBoundingBox(left, top, right, bottom);

	//if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < 2816)
	//{
	//	/*if (player->immovable)
	//	{
	//		camSpeedX = 0;
	//		goto SET_CAM;
	//	}
	//	camSpeedX = player->vx;*/
	//	camX = player->x - (SCREEN_WIDTH / 4);
	//}

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
		if (top + 7 < round(camY + SCREEN_HEIGHT / 4 - 20) && camY <= 200)
		{
			camSpeedY = player->vy;
		}
		else
			camSpeedY = 0;
	}

	lastPlayerX = player->x;

	SET_CAM:
	//camX += camSpeedX * dt;
	camY += camSpeedY * dt;

	//CGame::GetInstance()->SetCamPos(camX, camY);
	CGame::GetInstance()->SetCamPosY(camY);
}

Camera::~Camera()
{
}

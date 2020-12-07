#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
	//camX = playerX - (SCREEN_WIDTH / 4);
	//camY = CAMERA_INITIAL_Y;
	camY = 50;
	CGame::GetInstance()->SetCamPos(camX, camY);
}

void Camera::Update(ULONGLONG dt)
{
	//DebugOut(L"CAM y: %f\n", camY);
	//DebugOut(L"isWaggingTail = %d\n", player->isWaggingTail);
	//DebugOut(L"mario vy: %f\n", player->vy);
	float playerTop = player->GetTop();

	if (goToHiddenArea)
	{
		CGame::GetInstance()->SetCamPosY(CAMERA_HIDDEN_AREA_Y);
		player->y = MARIO_READY_TO_OUT_OF_PIPE_POS_Y;
		player->x = 2095;
		player->inEndOfPipe = true;
		inHiddenArea = true;
	}

	if (player->readyToOutOfPipe)
	{
		DebugOut(L"set pos MARIO\n");
		player->inEndOfPipe = false;
		player->SetState(MARIO_STATE_GO_INTO_PIPE);
		player->readyToOutOfPipe = false;
		goToHiddenArea = false;
		inHiddenArea = true;
		return;
	}

	if (inHiddenArea)
		return;

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

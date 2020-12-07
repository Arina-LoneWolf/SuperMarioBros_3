#include "Camera.h"

Camera::Camera(CMario* player)
{
	this->player = player;
	camY = CAMERA_INITIAL_Y;
	CGame::GetInstance()->SetCamPosY(camY);
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
		player->x = MARIO_READY_TO_OUT_OF_PIPE_POS_X;
		player->inEndOfPipe = true;
		inHiddenArea = true;
	}

	if (backFromHiddenArea)
	{
		CGame::GetInstance()->SetCamPosY(CAMERA_INITIAL_Y);
		player->y = MARIO_READY_TO_OUT_OF_HIDDEN_AREA_POS_Y;
		player->x = MARIO_READY_TO_OUT_OF_HIDDEN_AREA_POS_X;
		player->inEndOfPipe = true;
		//inHiddenArea = false;
	}

	if (player->readyToOutOfPipe)
	{
		player->inEndOfPipe = false;
		if (goToHiddenArea)
			player->SetState(MARIO_STATE_GO_INTO_PIPE);
		else if (backFromHiddenArea)
			player->SetState(MARIO_STATE_OUT_OF_PIPE);
		if (backFromHiddenArea)
			inHiddenArea = false;
		player->readyToOutOfPipe = false;
		goToHiddenArea = false;
		backFromHiddenArea = false;
		return;
	}

	if (inHiddenArea)
		return;

	if (!goToHiddenArea && player->GetState() == MARIO_STATE_GO_INTO_PIPE)
		return;

	if (player->GetState() == MARIO_STATE_OUT_OF_PIPE)
	{
		camY = CAMERA_INITIAL_Y;
		return;
	}

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
	/*if (camY <= 0 || camY > CAMERA_INITIAL_Y)
		return;*/
	if (camY < 0)
		camY = 0;
	if (camY > CAMERA_INITIAL_Y)
		camY = CAMERA_INITIAL_Y;

	CGame::GetInstance()->SetCamPosY(camY);
}

Camera::~Camera()
{
}

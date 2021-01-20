#include "BoomerangBrother.h"
#include "Mario.h"

CBoomerangBrother* CBoomerangBrother::__instance = nullptr;

CBoomerangBrother::CBoomerangBrother()
{
	category = Category::ENEMY;
	GenerateRandomTurnToJump();
	SetState(ENEMY_STATE_MOVE);
	coolDown->Start();
}

void CBoomerangBrother::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += BOOMERANG_BROTHER_GRAVITY * dt;

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / SCREEN_DIVISOR && listBoomerang.empty())
		isFinishedUsing = true;

	if (effect)
	{
		effect->Update(dt, coObjects);
		if (effect->isFinishedUsing)
			effect = nullptr;
	}

	if ((vx > 0 && x >= BOOMERANG_BROTHER_LIMIT_X_RIGHT) || (vx < 0 && x <= BOOMERANG_BROTHER_LIMIT_X_LEFT))
	{
		if (!died)
		{
			turnCounter++;
			SetState(ENEMY_STATE_IDLE);
		}
	}

	if (delayTimeToRest->IsTimeUp() && !died)
	{
		SetState(ENEMY_STATE_MOVE);
		delayTimeToRest->Stop();
	}

	if (turnCounter == randTurnToJump && state != ENEMY_STATE_IDLE && !died)
	{
		SetState(BOOMERANG_BROTHER_STATE_JUMP);
		turnCounter = 0;
	}

	#pragma region Weapon Processing

	if (listBoomerang.size() < 2)
	{
		if (CMario::GetInstance()->GetLeft() < 2224)
		{
			if (listBoomerang.empty() && coolDown->IsTimeUp() && CMario::GetInstance()->onBackyardPipe)
			{
				CreateBoomerang();
				coolDown->Stop();
				delayToHurlSecBoomerang->Start();
			}
			else if (!listBoomerang.empty() && delayToHurlSecBoomerang->IsTimeUp())
			{
				CreateBoomerang();
				delayToHurlSecBoomerang->Stop();
			}
		}
	}

	for (UINT i = 0; i < listBoomerang.size(); i++)
		listBoomerang[i]->Update(dt, coObjects);

	if (listBoomerang.size() == 2)
	{
		int countUsedBoomerang = 0;

		for (UINT i = 0; i < listBoomerang.size(); i++)
		{
			if (listBoomerang[i]->isFinishedUsing)
				countUsedBoomerang++;
		}

		if (countUsedBoomerang == 2)
		{
			listBoomerang.clear();
			coolDown->Start();
		}
	}

	#pragma endregion

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		if (ny < 0)
		{
			vy = 0;
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerangBrother::Render()
{
	playerPosition = GetPlayerDirection();

	if (died)
	{
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_DIE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_DIE_LEFT;
		goto RENDER;
	}
	else if (isHoldingBoomerang)
	{
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_ATTACK_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_ATTACK_LEFT;
		goto RENDER;
	}

	switch (state)
	{
	case ENEMY_STATE_IDLE:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_IDLE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_IDLE_LEFT;
		break;

	case BOOMERANG_BROTHER_STATE_JUMP:
	case ENEMY_STATE_MOVE:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_MOVE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_MOVE_LEFT;
		break;

	case ENEMY_STATE_DIE_BY_WEAPON:
	case ENEMY_STATE_ATTACKED_BY_TAIL:
	case BOOMERANG_BROTHER_STATE_DIE_BY_CRUSH:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_DIE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_DIE_LEFT;
		break;
	}
	
	RENDER:
	animation_set->at(ani)->Render(x, y);

	for (UINT i = 0; i < listBoomerang.size(); i++)
		listBoomerang[i]->Render();

	if (effect)
		effect->Render();

	//RenderBoundingBox();
}

void CBoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (died)
		return;

	l = x;
	t = y;
	r = l + BOOMERANG_BROTHER_BBOX_WIDTH;
	b = t + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ENEMY_STATE_IDLE:
		vx = 0;
		delayTimeToRest->Start();
		break;

	case ENEMY_STATE_MOVE:
		if (x < BOOMERANG_BROTHER_LIMIT_X_RIGHT)
			vx = BOOMERANG_BROTHER_SPEED_X;
		else
			vx = -BOOMERANG_BROTHER_SPEED_X;
		break;

	case BOOMERANG_BROTHER_STATE_JUMP:
		//DebugOut(L"JUMPPPP\n");
		vy = -BOOMERANG_BROTHER_JUMP_SPEED_Y;
		GenerateRandomTurnToJump();
		break;

	case ENEMY_STATE_DIE_BY_WEAPON:
		vx = BOOMERANG_BROTHER_DEFLECT_SPEED_X * object_colliding_nx;
		vy = -BOOMERANG_BROTHER_DEFLECT_SPEED_Y;
		if (object_colliding_nx > 0)
			effect = new CScoreEffect({ x - 1, y - 7 });
		else
			effect = new CScoreEffect({ x + 8, y - 7 });
		died = true;
		break;

	case ENEMY_STATE_ATTACKED_BY_TAIL:
		vx = BOOMERANG_BROTHER_DEFLECT_SPEED_X_BY_TAIL * object_colliding_nx;
		vy = -BOOMERANG_BROTHER_DEFLECT_SPEED_Y_BY_TAIL;
		if (object_colliding_nx > 0)
			effect = new CScoreEffect({ x + 1, y - 3 });
		else
			effect = new CScoreEffect({ x - 7, y - 3 });
		died = true;
		break;

	case BOOMERANG_BROTHER_STATE_DIE_BY_CRUSH:
		vx = 0;
		died = true;
		effect = new CScoreEffect({ x + 1, y - 7 });
		break;
	}
}

void CBoomerangBrother::CreateBoomerang()
{
	CBoomerang* boomerang = new CBoomerang(playerPosition);
	listBoomerang.push_back(boomerang);
}

void CBoomerangBrother::GenerateRandomTurnToJump()
{
	randTurnToJump = rand() % (MAX_TURN_TO_JUMP - MIN_TURN_TO_JUMP + 1) + MIN_TURN_TO_JUMP;
}

int CBoomerangBrother::GetPlayerDirection()
{
	if (CMario::GetInstance()->GetLeft() >= this->x)
		return 1;
	return -1;
}

CBoomerangBrother* CBoomerangBrother::GetInstance()
{
	if (__instance == NULL) __instance = new CBoomerangBrother();
	return __instance;
}

CBoomerangBrother::~CBoomerangBrother()
{
}

#include "BoomerangBrother.h"
#include "Mario.h"

CBoomerangBrother::CBoomerangBrother()
{
	category = Category::ENEMY;
	GenerateRandomTurnToJump();
	SetState(BOOMERANG_BROTHER_STATE_MOVE);

}

void CBoomerangBrother::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += BOOMERANG_BROTHER_GRAVITY * dt;

	if ((vx > 0 && x >= BOOMERANG_BROTHER_LIMIT_X_RIGHT) || (vx < 0 && x <= BOOMERANG_BROTHER_LIMIT_X_LEFT))
	{
		turnCounter++;
		SetState(BOOMERANG_BROTHER_STATE_IDLE);
	}

	if (delayTimeToRest->IsTimeUp())
	{
		SetState(BOOMERANG_BROTHER_STATE_MOVE);
		delayTimeToRest->Stop();
	}

	if (turnCounter == randTurnToJump && state != BOOMERANG_BROTHER_STATE_IDLE)
	{
		SetState(BOOMERANG_BROTHER_STATE_JUMP);
		turnCounter = 0;
	}

	#pragma region Weapon Processing

	if (listBoomerang.size() < 2)
	{
		if (CMario::GetInstance()->GetLeft() < 2224)
		{
			if (listBoomerang.empty() && coolDown->IsTimeUp())
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
			for (UINT i = 0; i < listBoomerang.size(); i++)
				listBoomerang.erase(listBoomerang.begin() + 1);

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

	switch (state)
	{
	case BOOMERANG_BROTHER_STATE_IDLE:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_IDLE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_IDLE_LEFT;
		break;

	case BOOMERANG_BROTHER_STATE_JUMP:
	case BOOMERANG_BROTHER_STATE_MOVE:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_MOVE_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_MOVE_LEFT;
		break;

	case BOOMERANG_BROTHER_STATE_ATTACK:
		if (playerPosition > 0)
			ani = BOOMERANG_BROTHER_ANI_ATTACK_RIGHT;
		else
			ani = BOOMERANG_BROTHER_ANI_ATTACK_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	for (UINT i = 0; i < listBoomerang.size(); i++)
		listBoomerang[i]->Render();

	RenderBoundingBox();
}

void CBoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b)
{
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
	case BOOMERANG_BROTHER_STATE_IDLE:
		vx = 0;
		delayTimeToRest->Start();
		break;

	case BOOMERANG_BROTHER_STATE_MOVE:
		if (x < BOOMERANG_BROTHER_LIMIT_X_RIGHT)
			vx = BOOMERANG_BROTHER_SPEED_X;
		else
			vx = -BOOMERANG_BROTHER_SPEED_X;
		break;

	case BOOMERANG_BROTHER_STATE_JUMP:
		DebugOut(L"JUMPPPP\n");
		vy = -BOOMERANG_BROTHER_JUMP_SPEED_Y;
		GenerateRandomTurnToJump();
		break;

	case BOOMERANG_BROTHER_STATE_ATTACK:
		break;
	}
}

void CBoomerangBrother::CreateBoomerang()
{
	CBoomerang* boomerang = new CBoomerang({ x, y }, playerPosition);
	listBoomerang.push_back(boomerang);
}

void CBoomerangBrother::GenerateRandomTurnToJump()
{
	randTurnToJump = rand() % (MAX_TURN_TO_JUMP - MIN_TURN_TO_JUMP + 1) + MIN_TURN_TO_JUMP;
}

bool CBoomerangBrother::GetPlayerDirection()
{
	return CMario::GetInstance()->GetLeft() >= this->x;
}

CBoomerangBrother::~CBoomerangBrother()
{
}

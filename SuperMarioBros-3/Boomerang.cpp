#include "Boomerang.h"
#include "Mario.h"
#include "BoomerangBrother.h"
//#include "Math.h"

CBoomerang::CBoomerang(int nx)
{
	type = Type::BOOMERANG;
	category = Category::WEAPON;

	CBoomerangBrother::GetInstance()->isHoldingBoomerang = true;

	SetPositionFollowBoomerangBrother();

	initialPosX = this->x;

	this->nx = nx;

	delayToHurl->Start();

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(27));
}

void CBoomerang::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (delayToHurl->IsTimeUp())
	{
		if (!limitPosY)
			limitPosY = this->y + BOOMERANG_LIMIT_POS_Y_ADDEND;

		if (y <= limitPosY && vy != 0)
			vy += BOOMERANG_GRAVITY * dt;
		else
			vy = 0;

		if (vy == 0 && vx == 0)
			SetState(BOOMERANG_STATE_HURLING);
	}

	x += dx;
	y += dy;

	if (state != BOOMERANG_STATE_HURLING)
		SetPositionFollowBoomerangBrother();

	if ((abs(x - initialPosX) > BOOMERANG_LIMIT_DX) || (nx > 0 && vx < 0) || (nx < 0 && vx > 0))
	{
		if (nx > 0)
		{
			vx -= BOOMERANG_ACCELERATION_TO_PULL_BACK * dt;
			if (vx <= -BOOMERANG_SPEED_X)
				vx = -BOOMERANG_SPEED_X;
		}
		else
		{
			vx += BOOMERANG_ACCELERATION_TO_PULL_BACK * dt;
			if (vx <= BOOMERANG_SPEED_X)
				vx = BOOMERANG_SPEED_X;
		}
	}
	
	float ml, mt, mr, mb, pl, pt, pr, pb; // main object - boomerang (m) and the player (p)
	GetBoundingBox(ml, mt, mr, mb);

	CMario::GetInstance()->GetBoundingBox(pl, pt, pr, pb);
	if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
	{
		if (!CMario::GetInstance()->GetUntouchable())
		{
			CreateEffect();
			CMario::GetInstance()->CollideWithEnemy();
		}
	}

	float bl, bt, br, bb;
	CBoomerangBrother::GetInstance()->GetBoundingBox(bl, bt, br, bb);
	if (CGameObject::CheckAABB(ml, mt, mr, mb, bl, bt, br, bb))
	{
		if ((nx > 0 && vx < 0) || (nx < 0 && vx > 0))
			isFinishedUsing = true;
	}

	if (x + BOOMERANG_BBOX_WIDTH < CGame::GetInstance()->GetCamPosX() || x > CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR)
		isFinishedUsing = true;

	for (UINT i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
		if (listEffect[i]->isFinishedUsing)
			listEffect.erase(listEffect.begin() + i);
	}
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BOOMERANG_BBOX_WIDTH;
	b = t + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::Render()
{
	if (isFinishedUsing)
		return;

	if (state == BOOMERANG_STATE_HURLING)
	{
		if (nx > 0)
			ani = BOOMERANG_ANI_MOVE_RIGHT;
		else
			ani = BOOMERANG_ANI_MOVE_LEFT;
	}
	else
	{
		if (nx > 0)
			ani = BOOMERANG_ANI_IDLE_RIGHT;
		else
			ani = BOOMERANG_ANI_IDLE_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	for (UINT i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}

	//RenderBoundingBox();
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == BOOMERANG_STATE_HURLING)
	{
		vy = -BOOMERANG_HURLING_SPEED_Y;
		if (nx > 0)
			vx = BOOMERANG_SPEED_X;
		else
			vx = -BOOMERANG_SPEED_X;
		CBoomerangBrother::GetInstance()->isHoldingBoomerang = false;//CBoomerangBrother::GetInstance()->SetState(BOOMERANG_BROTHER_STATE_NORMAL);
	}
}

void CBoomerang::CreateEffect()
{
	CHitEffect* effect = new CHitEffect({ x, y }, nx);
	listEffect.push_back(effect);
}

void CBoomerang::SetPositionFollowBoomerangBrother()
{
	float playerPosX = CBoomerangBrother::GetInstance()->x;
	float playerPosY = CBoomerangBrother::GetInstance()->y;

	if (nx > 0)
	{
		x = playerPosX - BOOMERANG_POS_X_SUBTRAHEND;
		y = playerPosY - BOOMERANG_POS_Y_SUBTRAHEND;
	}
	else
	{
		x = playerPosX + BOOMERANG_POS_X_ADDEND;
		y = playerPosY - BOOMERANG_POS_Y_SUBTRAHEND;
	}
}

CBoomerang::~CBoomerang()
{
}

#include "Fireball.h"

CFireball::CFireball(D3DXVECTOR2 position, int nx)
{
	type = Type::FIREBALL;
	category = Category::WEAPON;

	if (nx > 0)
	{
		x = position.x + 10;
		y = position.y + 6;
	}
	else
	{
		x = position.x - 6;
		y = position.y + 6;
	}

	this->nx = nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void CFireball::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (x > CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / 2 || x < CGame::GetInstance()->GetCamPosX())
		isFinishedUsing = true;

	if (!isFinishedUsing)
	{
		if (nx > 0)
			vx = FIREBALL_SPEED;
		else
			vx = -FIREBALL_SPEED;
		vy += (MARIO_GRAVITY * dt);
	}

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
		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;

		if (ny < 0)
		{
			vy = -FIREBALL_SPEED;
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->type == Type::COLOR_BOX)
			{
				if (e->nx != 0)
					x += dx;
			}
			else if (e->obj->category == Category::MISC)
			{
				if (e->nx != 0)
					isFinishedUsing = true;
			}
			else if (e->obj->category == Category::ENEMY)
			{
				e->obj->object_colliding_nx = this->nx;
				e->obj->SetState(ENEMY_STATE_DIE_BY_WEAPON);
				isFinishedUsing = true;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}

void CFireball::Render()
{
	if (nx > 0)
		animation_set->at(FIREBALL_TO_RIGHT)->Render(x, y);
	else
		animation_set->at(FIREBALL_TO_LEFT)->Render(x, y);
	//RenderBoundingBox();
}

CFireball::~CFireball()
{
}

#include "Koopa.h"

CKoopa::CKoopa()
{
	type = KOOPA;
	category = ENEMY;
	SetState(ENEMY_STATE_MOVE);
}

void CKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += MARIO_GRAVITY * dt;

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
		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.4f;

		if (ny != 0)
		{
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (state == ENEMY_STATE_MOVE)
			{
				if (e->obj->type == Type::COLOR_BOX)
				{
					if (e->nx != 0)
						x += dx;
				}
				else if (e->obj->category == Category::MISC)
				{
					if (e->nx != 0)
					{
						vx = -vx;
					}
					else if (e->ny != 0 && (x <= e->obj->x || x >= e->obj->x + e->obj->width))
					{
						vx = -vx;
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopa::Render()
{
	if (state == ENEMY_STATE_DIE)
		ani = KOOPA_ANI_LAY_SUPINE;
	//else if...

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_DIE:
		vx = KOOPA_DEFLECT_SPEED_X * attack_tool_nx;
		vy = -KOOPA_DEFLECT_SPEED_Y;
		isFinishedUsing = true;
		break;
	case ENEMY_STATE_MOVE:
		vx = KOOPA_MOVE_SPEED_X; // nhân thêm với nx của..?
	case ENEMY_STATE_IDLE:
		vx = 0;
		vy = 0;
	case KOOPA_STATE_SPIN_AND_MOVE:
		vx = KOOPA_SPIN_AND_MOVE_SPEED_X;
	}

}

CKoopa::~CKoopa()
{
}

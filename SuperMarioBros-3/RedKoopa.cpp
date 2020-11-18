#include "RedKoopa.h"

CRedKoopa::CRedKoopa(CMario* mario)
{
	type = KOOPA;
	category = ENEMY;
	player = mario;
	SetState(ENEMY_STATE_MOVE);
}

void CRedKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (died)
		return;
	left = x;
	right = x + KOOPA_BBOX_WIDTH;
	bottom = y + KOOPA_BBOX_HEIGHT;

	if (state == ENEMY_STATE_MOVE)
		top = y;
	else
		top = y + (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_LAY_VIBRATE_SPIN);
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (isBeingHeld && !player->isHoldingShell)
	{
		if (player->nx > 0)
			object_colliding_nx = 1;
		else
			object_colliding_nx = -1;
		x += 8 * object_colliding_nx;
		SetState(KOOPA_STATE_SPIN_AND_MOVE);
		isBeingHeld = false;
	}

	if (state != KOOPA_STATE_BEING_HELD)
		vy += MARIO_GRAVITY * dt;
	else
	{
		SetPositionAccordingToPlayer();
	}

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / 2)
		isFinishedUsing = true;

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

			if (state == ENEMY_STATE_MOVE || state == KOOPA_STATE_SPIN_AND_MOVE)
			{
				if (e->obj->type == Type::COLOR_BOX)
				{
					if (e->nx != 0)
						x += dx;
				}
				if (e->obj->category == Category::MISC)
				{
					if (e->nx != 0 && e->obj->type != Type::COLOR_BOX)
					{
						vx = -vx;
					}
					if (e->ny != 0 && state == ENEMY_STATE_MOVE && (x <= e->obj->x || x >= e->obj->x + e->obj->width - KOOPA_BBOX_WIDTH))
					{
						vx = -vx;
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CRedKoopa::Render()
{
	switch (state)
	{
	case ENEMY_STATE_DIE_BY_WEAPON:
		ani = KOOPA_ANI_LAY_SUPINE;
		break;

	case ENEMY_STATE_IDLE:
		if (isSupine)
			ani = KOOPA_ANI_LAY_SUPINE;
		else
			ani = KOOPA_ANI_LAY_PRONE;
		break;

	case ENEMY_STATE_MOVE:
		if (vx > 0)
			ani = KOOPA_ANI_MOVE_RIGHT;
		else
			ani = KOOPA_ANI_MOVE_LEFT;
		break;

	case KOOPA_STATE_SPIN_AND_MOVE:
		if (isSupine)
			ani = KOOPA_ANI_SPIN_AND_MOVE_SUPINE;
		else
			ani = KOOPA_ANI_SPIN_AND_MOVE_PRONE;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CRedKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_DIE_BY_WEAPON:
		vx = KOOPA_DEFLECT_SPEED_X * object_colliding_nx;
		vy = -KOOPA_DEFLECT_SPEED_Y;
		died = true;
		break;
	case ENEMY_STATE_ATTACKED_BY_TAIL:
		isSupine = true;
		vx = ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL * object_colliding_nx;
		vy = -ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL;
	case ENEMY_STATE_MOVE:
		vx = -KOOPA_MOVE_SPEED_X;
		break;
	case KOOPA_STATE_BEING_HELD:
		isBeingHeld = true;
	case ENEMY_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_SPIN_AND_MOVE:
		vx = KOOPA_SPIN_AND_MOVE_SPEED_X * object_colliding_nx;
		break;
	}
}

void CRedKoopa::SetPositionAccordingToPlayer()
{
	if (player->GetLevel() == MARIO_LEVEL_BIG)
	{
		if (player->nx > 0)
		{
			x = player->x + 17;
			y = player->y - 2;
		}
		else
		{
			x = player->x - 11;
			y = player->y - 2;
		}
	}
	else if (player->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (player->nx > 0)
		{
			x = player->x + 10;
			y = player->y - 12;
		}
		else
		{
			x = player->x - 11;
			y = player->y - 12;
		}
	}
	else if (player->GetLevel() == MARIO_RACCOON)
	{
		if (player->nx > 0)
		{
			x = player->x + 17;
			y = player->y - 2;
		}
		else
		{
			x = player->x - 2;
			y = player->y - 2;
		}
	}
	else if (player->GetLevel() == MARIO_FIRE)
	{
		if (player->nx > 0)
		{
			x = player->x + 10;
			y = player->y - 2;
		}
		else
		{
			x = player->x - 11;
			y = player->y - 2;
		}
	}
}

﻿#include "SuperMushroom.h"

CSuperMushroom::CSuperMushroom(float brickX, float brickY)
{
	type = SUPER_MUSHROOM;
	category = ITEM;

	this->x = brickX;
	this->y = brickY;
	vy = -SUPER_MUSHROOM_DEFLECT_SPEED_Y;

	minPosY = brickY - SUPER_MUSHROOM_BBOX_HEIGHT; // -1?
	/*leftLimit = brickX;
	rightLimit = brickX + 32;*/

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (y <= minPosY)
		isFalling = true; // stopBouncing

	if (isFalling)
	{
		if (!isdone)
		{
			vx = SUPER_MUSHROOM_SPEED_X;
		}
		vy = SUPER_MUSHROOM_SPEED_Y;
		isdone = true;
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
		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.1f;

		if (ny < 0)
		{
			vy = 0;
		}

		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->type == Type::COLOR_BOX) // tạm thôi, còn sửa
			{
				if (e->nx != 0)
					x += dx;
			}
			else if (e->obj->category == Category::MISC)
			{
				if (e->nx != 0)
					vx = -vx;
			}
			else if (e->obj->category == Category::ENEMY)
			{
				x += dx;
				y += dy;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSuperMushroom::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CSuperMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SUPER_MUSHROOM_BBOX_WIDTH;
	b = y + SUPER_MUSHROOM_BBOX_HEIGHT;
}

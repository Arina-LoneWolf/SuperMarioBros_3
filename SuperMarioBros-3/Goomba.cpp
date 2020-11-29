#include "Goomba.h"

CGoomba::CGoomba(CMario* player)
{
	category = Category::ENEMY;
	this->player = player;
	SetState(ENEMY_STATE_MOVE); 
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (died)
		return;
	left = x;
	right = x + GOOMBA_BBOX_WIDTH;
	bottom = y + GOOMBA_BBOX_HEIGHT;

	if (state == GOOMBA_STATE_DIE_BY_CRUSH)
		top = y + (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE_BY_CRUSH);
	else
		top = y;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += MARIO_GRAVITY * dt; // cho gravity nhỏ lại

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / 2)
		isFinishedUsing = true;

	if (deadTime->IsTimeUp())
		isFinishedUsing = true;

	if (effect)
		effect->Update(dt, coObjects);

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
				else if (e->obj->category == Category::MISC || e->obj->category == Category::ENEMY)
				{
					if (e->nx != 0)
					{
						vx = -vx;
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	if (state == ENEMY_STATE_DIE_BY_WEAPON || state == ENEMY_STATE_ATTACKED_BY_TAIL)
		ani = GOOMBA_ANI_DIE_BY_ATTACK_TOOL;
	else if (state == GOOMBA_STATE_DIE_BY_CRUSH)
		ani = GOOMBA_ANI_DIE_BY_CRUSH;
	else
		ani = GOOMBA_ANI_MOVE;

	animation_set->at(ani)->Render(x, y);

	if (effect)
		effect->Render();

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_DIE_BY_WEAPON:
		vx = GOOMBA_DEFLECT_SPEED_X * object_colliding_nx;
		vy = -GOOMBA_DEFLECT_SPEED_Y;
		if (object_colliding_nx > 0)
			effect = new CMoneyEffect({ x - 1, y - 7 });
		else
			effect = new CMoneyEffect({ x + 8, y - 7 });
		died = true;
		break;

	case ENEMY_STATE_ATTACKED_BY_TAIL:
		vx = ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL * object_colliding_nx;
		vy = -ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL;
		if (object_colliding_nx > 0)
			effect = new CMoneyEffect({ x + 1, y - 3 });
		else
			effect = new CMoneyEffect({ x - 7, y - 3 });
		died = true;
		break;

	case ENEMY_STATE_MOVE:
		vx = -GOOMBA_MOVE_SPEED_X;
		nx = -1;
		break;

	case GOOMBA_STATE_DIE_BY_CRUSH:
		vx = 0;
		deadTime->Start();
		effect = new CMoneyEffect({ x + 3, y - 7 });
		break;

	case GOOMBA_STATE_FLY_LOW:
		vy = -0.05f;
		break;

	case GOOMBA_STATE_FLY_HIGH:
		vy = -0.5;
		break;
	}
}

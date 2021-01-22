#include "Goomba.h"

CGoomba::CGoomba(CMario* player, Type goombaType)
{
	category = Category::ENEMY;
	this->player = player;
	type = goombaType;
	if (type == Type::YELLOW_GOOMBA)
		SetState(ENEMY_STATE_MOVE); 
}

float CGoomba::GetSpeedX()
{
	if (player->GetLeft() > this->x)
		return GOOMBA_MOVE_SPEED_X;
	else
		return -GOOMBA_MOVE_SPEED_X;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (type == Type::RED_PARAGOOMBA)
	{
		left = x + GOOMBA_LEFT_ADDEND;
		top = y + GOOMBA_TOP_ADDEND;
	}
	else
	{
		left = x;
		top = y;
	}
	if (state == GOOMBA_STATE_DIE_BY_CRUSH)
		top = y + (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE_BY_CRUSH);
	right = left + GOOMBA_BBOX_WIDTH;
	bottom = top + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (deadTime->IsStopped())
	{
		if (state == GOOMBA_STATE_FLY_HIGH)
			vy += GOOMBA_HIGH_FLYING_GRAVITY * dt;
		else
			vy += GOOMBA_GRAVITY * dt;
	}

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / SCREEN_DIVISOR + 250)
		isFinishedUsing = true;

	if (type == Type::RED_PARAGOOMBA)
	{
		if (CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR >= this->x && !state)
		{
			SetState(ENEMY_STATE_MOVE);
			interestedInChasing->Start();
		}
	}

	if (deadTime->IsTimeUp())
		isFinishedUsing = true;

	if (state == ENEMY_STATE_MOVE && delayToRedirectAgain->IsTimeUp() && !lostWings && !interestedInChasing->IsTimeUp()/* || vx < 0*/)
	{
		if (type == Type::RED_PARAGOOMBA)
		{
			vx = GetSpeedX();
			delayToRedirectAgain->Stop();
		}
	}

	if (walkTime->IsTimeUp() && !died)
	{
		SetState(GOOMBA_STATE_FLY_LOW);
		walkTime->Stop();
	}

	if (effect)
		effect->Update(dt, coObjects);

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (!died)
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
			if (state == GOOMBA_STATE_FLY_LOW && lowFlyingCounter < 3)
				SetState(GOOMBA_STATE_FLY_LOW);
			else if (state == GOOMBA_STATE_FLY_LOW && lowFlyingCounter == 3)
				SetState(GOOMBA_STATE_FLY_HIGH);
			else if (state == GOOMBA_STATE_FLY_HIGH)
				SetState(ENEMY_STATE_MOVE);
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (state == ENEMY_STATE_MOVE || state == GOOMBA_STATE_FLY_LOW || state == GOOMBA_STATE_FLY_HIGH || state == GOOMBA_STATE_NORMAL)
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
						e->obj->vx = -e->obj->vx;
					}
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	DebugOut(L"state: %d\n", state);
}

void CGoomba::Render()
{
	if (type == Type::RED_PARAGOOMBA)
	{
		if (state == ENEMY_STATE_DIE_BY_WEAPON || state == ENEMY_STATE_ATTACKED_BY_TAIL)
			ani = RED_PARAGOOMBA_ANI_DIE_BY_ATTACK_TOOL;
		else if (state == GOOMBA_STATE_DIE_BY_CRUSH)
			ani = RED_PARAGOOMBA_ANI_DIE_BY_CRUSH;
		else if (state == ENEMY_STATE_MOVE || state == GOOMBA_STATE_NORMAL)
		{
			if (lostWings)
				ani = RED_PARAGOOMBA_ANI_MOVE_WHEN_LOSING_WINGS;
			else
				ani = RED_PARAGOOMBA_ANI_MOVE;
		}
		else if (state == GOOMBA_STATE_FLY_LOW)
			ani = RED_PARAGOOMBA_ANI_FLAP_WINGS_SLOWLY;
		else if (state == GOOMBA_STATE_FLY_HIGH)
			ani = RED_PARAGOOMBA_ANI_FLAP_WINGS_QUICKLY;
	}
	else/* if (type == Type::YELLOW_GOOMBA)*/
	{
		if (state == ENEMY_STATE_DIE_BY_WEAPON || state == ENEMY_STATE_ATTACKED_BY_TAIL)
			ani = GOOMBA_ANI_DIE_BY_ATTACK_TOOL;
		else if (state == GOOMBA_STATE_DIE_BY_CRUSH)
			ani = GOOMBA_ANI_DIE_BY_CRUSH;
		else
			ani = GOOMBA_ANI_MOVE;
	}

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
			effect = new CScoreEffect({ x - 1, y - 7 });
		else
			effect = new CScoreEffect({ x + 8, y - 7 });
		died = true;
		break;

	case ENEMY_STATE_ATTACKED_BY_TAIL:
		vx = ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL * object_colliding_nx;
		vy = -ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL;
		if (object_colliding_nx > 0)
			effect = new CScoreEffect({ x + 1, y - 3 });
		else
			effect = new CScoreEffect({ x - 7, y - 3 });
		died = true;
		break;

	case GOOMBA_STATE_NORMAL:
		lostWings = true;
		break;

	case ENEMY_STATE_MOVE:
		if (type == Type::RED_PARAGOOMBA)
		{
			if (!interestedInChasing->IsTimeUp())
				vx = GetSpeedX();
			walkTime->Start();
			delayToRedirectAgain->Start();
		}
		else
			vx = -GOOMBA_MOVE_SPEED_X;
		nx = -1;
		break;

	case GOOMBA_STATE_DIE_BY_CRUSH:
		vx = 0;
		vy = 0;
		deadTime->Start();
		effect = new CScoreEffect({ x + 1, y - 7 });
		break;

	case GOOMBA_STATE_FLY_LOW:
		vy = -GOOMBA_LOW_FLYING_DEFECT_SPEED_Y;
		lowFlyingCounter++;
		break;

	case GOOMBA_STATE_FLY_HIGH:
		lowFlyingCounter = 0;
		vy = -GOOMBA_HIGH_FLYING_DEFECT_SPEED_Y;
		break;
	}
}

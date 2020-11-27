#include "Koopa.h"

CKoopa::CKoopa(CMario* mario, int startingPos, int koopaType)
{
	if (koopaType == Type::RED_KOOPA)
		type = RED_KOOPA;
	else if (koopaType == Type::GREEN_KOOPA)
		type = GREEN_KOOPA;
	else
		type = GREEN_PARAKOOPA;
	category = ENEMY;
	player = mario;
	this->startingPos = startingPos;
	SetState(ENEMY_STATE_MOVE);
}

void CKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (died)
		return;
	left = x + KOOPA_LEFT_ADDEND;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = y + KOOPA_BBOX_HEIGHT;

	if (state == ENEMY_STATE_MOVE)
		top = y;
	else
		top = y + (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_LAY_VIBRATE_SPIN);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	float resetPosX;
	switch (startingPos)
	{
	case ON_GREEN_COLOR_BOX:
		resetPosX = GCB_KOOPA_POS_X;
		break;
	case ON_PURPLE_COLOR_BOX:
		resetPosX = PCB_KOOPA_POS_X;
		break;
	case ON_BRONZE_BRICK:
		resetPosX = BB_KOOPA_POS_X;
		break;
	case ON_GROUND:
		resetPosX = G_KOOPA_POS_X;
		break;
	}
	if (type == Type::RED_KOOPA)
	{
		if (x > CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / 2)
			Reset();
		else if (CGame::GetInstance()->GetCamPosX() > resetPosX + KOOPA_BBOX_HEIGHT)
		{
			//if (!isBeingHeld)
			if (CGame::GetInstance()->GetCamPosX() > x + KOOPA_BBOX_WIDTH)
				Reset();
		}
	}

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / 2)
		reset = true;

	if (type != Type::RED_KOOPA && reset)
	{
		if (CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / 2 < resetPosX || CGame::GetInstance()->GetCamPosX() > resetPosX + KOOPA_BBOX_HEIGHT)
			Reset();
	}

	if (isBeingHeld && !player->isHoldingShell)
	{
		if (player->nx > 0)
			object_colliding_nx = 1;
		else
			object_colliding_nx = -1;
		x += 6 * object_colliding_nx;

		player->kickShell = true;
		player->kickStartTime = GetTickCount64();
		SetState(KOOPA_STATE_SPIN_AND_MOVE);
		isBeingHeld = false;
	}

	if (sleepStartTime && GetTickCount64() - sleepStartTime > KOOPA_SLEEP_TIME)
	{
		SetState(KOOPA_STATE_VIBRATE);
		sleepStartTime = 0;
	}

	if (vibrationStartTime && GetTickCount64() - vibrationStartTime > KOOPA_VIBRATION_TIME)
	{
		SetState(ENEMY_STATE_MOVE);
		vibrationStartTime = 0;
		player->isHoldingShell = false;
	}

	if (state == KOOPA_STATE_VIBRATE)
	{
		if (isBeingHeld)
			vy = 0;
		else
			vy += MARIO_GRAVITY * dt;
	}
	else if (state != KOOPA_STATE_BEING_HELD)
			vy += MARIO_GRAVITY * dt;
	
	if (isBeingHeld)
		SetPositionAccordingToPlayer();

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
			if (type == Type::GREEN_PARAKOOPA)
				vy = -GREEN_PARAKOOPA_DEFLECT_SPEED_Y;
			else
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
						if (type == Type::RED_KOOPA)
							vx = -vx;
					}
				}
				if (e->obj->category == Category::ENEMY && state == KOOPA_STATE_SPIN_AND_MOVE)
				{
					if (e->nx != 0)
					{
						if (vx > 0)
							e->obj->object_colliding_nx = 1;
						else
							e->obj->object_colliding_nx = -1;
						
						e->obj->SetState(ENEMY_STATE_DIE_BY_WEAPON);
					}
				}
				if (e->obj->type == Type::BRICK_CONTAINS_ITEM && state == KOOPA_STATE_SPIN_AND_MOVE)
				{
					e->obj->SetState(STATE_RAMMED);
				}
			}
			else if (state == ENEMY_STATE_ATTACKED_BY_TAIL)
			{
				if (e->obj->type == Type::COLOR_BOX)
				{
					if (e->ny > 0)
						y += dy;
				}
				if (e->obj->category == Category::MISC)
				{
					if (e->ny < 0)
						SetState(ENEMY_STATE_IDLE);
				}
			}
		}
	}

	if (abs(vx) == KOOPA_MOVE_SPEED_X)
		lastMoveSpeed = vx;

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopa::Render()
{
	switch (state)
	{
	case ENEMY_STATE_ATTACKED_BY_TAIL:
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
		if (type == Type::GREEN_PARAKOOPA)
		{
			if (vx > 0)
				ani = GREEN_PARAKOOPA_ANI_MOVE_RIGHT;
			else
				ani = GREEN_PARAKOOPA_ANI_MOVE_LEFT;
		}
		else
		{
			if (vx > 0)
				ani = KOOPA_ANI_MOVE_RIGHT;
			else
				ani = KOOPA_ANI_MOVE_LEFT;
		}
		break;

	case KOOPA_STATE_SPIN_AND_MOVE:
		if (isSupine)
			ani = KOOPA_ANI_SPIN_AND_MOVE_SUPINE;
		else
			ani = KOOPA_ANI_SPIN_AND_MOVE_PRONE;
		break;

	case KOOPA_STATE_VIBRATE:
		if (isSupine)
			ani = KOOPA_ANI_VIBRATE_SUPINE;
		else
			ani = KOOPA_ANI_VIBRATE_PRONE;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	if (effect)
		effect->Render();

	//RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_DIE_BY_WEAPON:
		vx = KOOPA_DEFLECT_SPEED_X * object_colliding_nx;
		vy = -KOOPA_DEFLECT_SPEED_Y;
		if (object_colliding_nx > 0)
			effect = new CMoneyEffect({ x - 1, y - 7 });
		else
			effect = new CMoneyEffect({ x + 8, y - 7 });
		died = true;
		break;

	case ENEMY_STATE_ATTACKED_BY_TAIL:
		isSupine = true;
		vx = ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL * object_colliding_nx;
		vy = -ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL;
		if (object_colliding_nx > 0)
			effect = new CMoneyEffect({ x + 1, y - 3 });
		else
			effect = new CMoneyEffect({ x - 7, y - 3 });
		break;

	case ENEMY_STATE_MOVE:
		if (type == Type::GREEN_PARAKOOPA)
			vx = -GREEN_PARAKOOPA_MOVE_SPEED_X;
		else if (type == Type::GREEN_KOOPA)
		{
			if (reset)
				vx = -KOOPA_MOVE_SPEED_X;
			else
				vx = -lastMoveSpeed;
		}
		else
			vx = -lastMoveSpeed;
		isBeingHeld = false;
		break;

	case KOOPA_STATE_BEING_HELD:
		isBeingHeld = true;
		break;

	case ENEMY_STATE_IDLE:
		vx = 0;
		vy = 0;
		sleepStartTime = GetTickCount64();
		break;

	case KOOPA_STATE_SPIN_AND_MOVE:
		vx = KOOPA_SPIN_AND_MOVE_SPEED_X * object_colliding_nx;
		isBeingHeld = false;
		sleepStartTime = 0;
		break;

	case KOOPA_STATE_VIBRATE:
		vibrationStartTime = GetTickCount64();
		break;

	case KOOPA_STATE_NORMAL:
		type = GREEN_KOOPA;
		SetState(ENEMY_STATE_MOVE);
		break;
	}
}

void CKoopa::SetPositionAccordingToPlayer()
{
	if (player->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (player->nx > 0)
			SetPosition(player->x + 19, player->y + 2);
		else
			SetPosition(player->x - 3, player->y + 2);
	}
	else if (player->GetLevel() == MARIO_RACCOON)
	{
		if (player->nx > 0)
			SetPosition(player->x + 19, player->y);
		else
			SetPosition(player->x - 3, player->y);
	}
	else if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_FIRE)
	{
		if (player->nx > 0)
			SetPosition(player->x + 19, player->y);
		else
			SetPosition(player->x - 3, player->y);
	}
}

void CKoopa::Reset()
{
	switch (startingPos)
	{
	case ON_GREEN_COLOR_BOX:
		SetPosition(GCB_KOOPA_POS_X, GCB_KOOPA_POS_Y);
		break;
	case ON_PURPLE_COLOR_BOX:
		type = GREEN_PARAKOOPA;
		SetPosition(PCB_KOOPA_POS_X, PCB_KOOPA_POS_Y);
		break;
	case ON_BRONZE_BRICK:
		SetPosition(BB_KOOPA_POS_X, BB_KOOPA_POS_Y);
		break;
	case ON_GROUND:
		SetPosition(G_KOOPA_POS_X, G_KOOPA_POS_Y);
		break;
	}

	SetState(ENEMY_STATE_MOVE);
	isSupine = false;
	reset = false;
	sleepStartTime = 0;
	vibrationStartTime = 0;
}

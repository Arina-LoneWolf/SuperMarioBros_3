#include "Koopa.h"

CKoopa::CKoopa(CMario* mario, float x, float y, Type type)
{
	category = Category::ENEMY;
	player = mario;
	startingPosX = x;
	startingPosY = y;
	initialType = type;
	this->type = type;
	SetState(ENEMY_STATE_MOVE);
}

void CKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == KOOPA_STATE_SPIN_AND_MOVE)
	{
		left = x + 2;
		right = left + 14;
	}
	else
	{
		left = x + KOOPA_LEFT_ADDEND;
		right = left + KOOPA_BBOX_WIDTH;
	}
	bottom = y + KOOPA_BBOX_HEIGHT;

	if (state == ENEMY_STATE_MOVE)
		top = y;
	else
		top = y + (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_LAY_VIBRATE_SPIN) + 1;
}

void CKoopa::Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (type == Type::RED_KOOPA)
	{
		if (x > CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR)
		{
			if (CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR < startingPosX)
				Reset();
		}
		else if (CGame::GetInstance()->GetCamPosX() > startingPosX + KOOPA_BBOX_HEIGHT)
		{
			if (CGame::GetInstance()->GetCamPosX() > x + KOOPA_BBOX_WIDTH)
				Reset();
		}
	}

	float camPosY = CGame::GetInstance()->GetCamPosY();
	if (camPosY && y > camPosY + SCREEN_HEIGHT / SCREEN_DIVISOR)
		reset = true;

	if (type != Type::RED_KOOPA && reset)
	{
		if (CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR < startingPosX || CGame::GetInstance()->GetCamPosX() > startingPosX + KOOPA_BBOX_HEIGHT)
			Reset();
	}

	if (type == Type::RED_PARAKOOPA)
	{
		if (vy == 0)
			vy = 0.05f;
		if ((vy > 0 && y >= 163) || (vy < 0 && y <= 52))
			vy = -vy;
	}

	if (isBeingHeld && !player->isHoldingShell)
	{
		if (player->nx > 0)
			object_colliding_nx = 1;
		else
			object_colliding_nx = -1;
		x += KOOPA_THOWN_OUT_DX * object_colliding_nx;

		player->kickShell = true;
		player->kickTime->Start();
		SetState(KOOPA_STATE_SPIN_AND_MOVE);
		isBeingHeld = false;
	}

	if (sleepTime->IsTimeUp())
	{
		SetState(KOOPA_STATE_VIBRATE);
		sleepTime->Stop();
	}

	if (vibrationTime->IsTimeUp())
	{
		SetState(ENEMY_STATE_MOVE);
		vibrationTime->Stop();
		player->isHoldingShell = false;
	}

	if (state == KOOPA_STATE_VIBRATE)
	{
		if (isBeingHeld)
			vy = 0;
		else
			vy += MARIO_GRAVITY * dt;
	}
	else if (state == KOOPA_STATE_SPIN_AND_MOVE)
		vy += 0.0015f * dt;
	else if (state != KOOPA_STATE_BEING_HELD && type != Type::RED_PARAKOOPA)
	{
		vy += MARIO_GRAVITY * dt;
	}

	if (isBeingHeld)
		SetPositionAccordingToPlayer();

	if (effect)
	{
		effect->Update(dt, coObjects);
		if (effect->isFinishedUsing)
			effect = nullptr;
	}

	if (type == Type::RED_KOOPA)
	{
		if (vx > 0)
		{
			virtualBoxBelow->SetPosition(x + 8, y + KOOPA_BBOX_HEIGHT);
			virtualBoxHorizontally->SetPosition(x + 11.1f, y + 16);
		}
		else
		{
			virtualBoxBelow->SetPosition(x + 3, y + KOOPA_BBOX_HEIGHT);
			virtualBoxHorizontally->SetPosition(x - 1.1f, y + 16);
		}

		if (virtualBoxBelow->CollideWithGround(coObjects, state))
		{
			vx = -vx;
			//DebugOut(L"Groundddddddddddddddd\n");
		}

		if (!virtualBoxBelow->CollideWithBrick(coObjects) && state == ENEMY_STATE_MOVE && onBrick)
		{
			vx = -vx;
			//DebugOut(L"not colliding\n");
		}

		if (virtualBoxHorizontally->CollidedHorizontally(coObjects, state))
		{
			vx = -vx;
			//DebugOut(L"Horizontallllllllllllllll\n");
		}
	}

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
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.2f;

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

		onBrick = false;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (state == ENEMY_STATE_MOVE || state == KOOPA_STATE_SPIN_AND_MOVE)
			{
				if (e->obj->type == Type::BRONZE_BRICK || e->obj->type == Type::BRICK_CONTAINS_ITEM)
				{
					if (e->ny < 0)
						onBrick = true;
				}
				if (e->obj->type == Type::COIN)
				{
					if (e->nx != 0)
						x += dx;
					else if (e->ny != 0)
						y += dy;
				}
				if (e->obj->type == Type::COLOR_BOX)
				{
					if (e->nx != 0)
						x += dx;
					if (e->ny < 0 && state == ENEMY_STATE_MOVE && (this->x + 8 <= e->obj->x || this->x >= e->obj->x + e->obj->width - KOOPA_BBOX_WIDTH + 8))
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
				if (e->obj->category == Category::ENEMY && state == ENEMY_STATE_MOVE)
				{
					if (e->nx != 0 && e->obj->state != KOOPA_STATE_SPIN_AND_MOVE)
					{
						vx = -vx;
						e->obj->vx = -e->obj->vx;
					}
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

	case KOOPA_STATE_NORMAL:
	case ENEMY_STATE_MOVE:
		if (type == Type::GREEN_PARAKOOPA || type == Type::RED_PARAKOOPA)
		{
			if (vx > 0)
				ani = PARAKOOPA_ANI_MOVE_RIGHT;
			else
				ani = PARAKOOPA_ANI_MOVE_LEFT;
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

	//virtualBoxBelow->Render();
	//virtualBoxHorizontally->Render();

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
			effect = new CScoreEffect({ x - 1, y - 7 });
		else
			effect = new CScoreEffect({ x + 8, y - 7 });
		died = true;
		break;

	case ENEMY_STATE_ATTACKED_BY_TAIL:
		isSupine = true;
		vx = ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL * object_colliding_nx;
		vy = -ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL;
		/*if (object_colliding_nx > 0)
			effect = new CScoreEffect({ x + 1, y - 3 });
		else
			effect = new CScoreEffect({ x - 7, y - 3 });*/
		break;

	case ENEMY_STATE_MOVE:
		if (type == Type::GREEN_PARAKOOPA)
			vx = -GREEN_PARAKOOPA_MOVE_SPEED_X;
		else if (type == Type::GREEN_KOOPA)
		{
			if (reset)
				vx = -KOOPA_MOVE_SPEED_X;
			else if (!justLostWings)
				vx = -lastMoveSpeed;
			else
				justLostWings = false;
		}
		else if (type == Type::RED_KOOPA)
			vx = -lastMoveSpeed;
		isBeingHeld = false;
		isSupine = false;
		break;

	case KOOPA_STATE_BEING_HELD:
		isBeingHeld = true;
		break;

	case ENEMY_STATE_IDLE:
		vx = 0;
		vy = 0;
		sleepTime->Start();
		break;

	case KOOPA_STATE_SPIN_AND_MOVE:
		vx = KOOPA_SPIN_AND_MOVE_SPEED_X * object_colliding_nx;
		isBeingHeld = false;
		sleepTime->Stop();
		vibrationTime->Stop();
		break;

	case KOOPA_STATE_VIBRATE:
		vibrationTime->Start();
		break;

	case KOOPA_STATE_NORMAL:
		if (type == Type::GREEN_PARAKOOPA)
			type = Type::GREEN_KOOPA;
		else if (type == Type::RED_PARAKOOPA)
			type = Type::RED_KOOPA;
		justLostWings = true;
		if (initialType != Type::RED_PARAKOOPA)
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
	type = initialType;
	SetPosition(startingPosX, startingPosY);
	SetState(ENEMY_STATE_MOVE);
	isSupine = false;
	reset = false;
	sleepTime->Stop();
	vibrationTime->Stop();
}

void CKoopa::MakeEffectWhenSteppedOn()
{
	effect = new CScoreEffect({ x + 1, y - 7 });
}

CKoopa::~CKoopa()
{
}

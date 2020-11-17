#include "FirePiranha.h"

CFirePiranha::CFirePiranha(CMario* mario, int piranhaType)
{
	category = ENEMY;
	type = FIRE_PIRANHA;
	player = mario;
	this->piranhaType = piranhaType;

	SetAreaLimit();
	SetState(FIRE_PIRANHA_STATE_MOVE_UP);
}

void CFirePiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	for (int i = 0; i < listFireball.size(); i++)
	{
		if (listFireball[i]->isFinishedUsing)
			listFireball.erase(listFireball.begin() + i);
	}

	if (delayToAttackStartTime && GetTickCount() - delayToAttackStartTime > FIRE_PIRANHA_DELAY_TO_ATTACK_TIME)
	{
		if (playerArea != Area::OUTSIDE_AREA)
			CreateFireball();
		delayToAttackStartTime = 0;
	}

	for (LPGAMEOBJECT fireball : listFireball)
		fireball->Update(dt, coObjects);

	if (!attackStartTime && y <= minPosY)
	{
		y = minPosY;
		vy = 0;
		attackStartTime = GetTickCount();
		delayToAttackStartTime = GetTickCount();
		SetState(FIRE_PIRANHA_STATE_ATTACK);
	}
	else if (!sleepStartTime && y >= FIRE_PIRANHA_MAX_Y)
	{
		y = FIRE_PIRANHA_MAX_Y;
		vy = 0;
		sleepStartTime = GetTickCount();
	}

	if (attackStartTime && GetTickCount() - attackStartTime > FIRE_PIRANHA_DELAY_TIME)
	{
		attackStartTime = 0;
		SetState(FIRE_PIRANHA_STATE_MOVE_DOWN);
	}

	if (sleepStartTime && GetTickCount() - sleepStartTime > FIRE_PIRANHA_DELAY_TIME)
	{
		if (!CheckPlayerInSafeZone(playerLeft, playerTop, playerRight, playerBottom))
		{
			sleepStartTime = 0;
			SetState(FIRE_PIRANHA_STATE_MOVE_UP);
		}
	}
}

void CFirePiranha::Render()
{
	playerArea = GetCurrentPlayerArea();

	if (state == FIRE_PIRANHA_STATE_ATTACK)
	{
		switch (playerArea)
		{
		case TOP_LEFT_FAR:
		case TOP_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_UP_LEFT;
			break;
		case TOP_RIGHT_FAR:
		case TOP_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_UP_RIGHT;
			break;
		case BOTTOM_LEFT_FAR:
		case BOTTOM_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;
			break;
		case BOTTOM_RIGHT_FAR:
		case BOTTOM_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_RIGHT;
			break;
		case OUTSIDE_AREA:
			ani = last_attack_ani;
			break;
		}
		last_attack_ani = ani;
	}
	else
	{
		switch (playerArea)
		{
		case TOP_LEFT_FAR:
		case TOP_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_UP_LEFT;
			break;
		case TOP_RIGHT_FAR:
		case TOP_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_UP_RIGHT;
			break;
		case BOTTOM_LEFT_FAR:
		case BOTTOM_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
			break;
		case BOTTOM_RIGHT_FAR:
		case BOTTOM_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_DOWN_RIGHT;
			break;
		case OUTSIDE_AREA:
			ani = last_face_ani;
			break;
		}
		last_face_ani = ani;
	}

	animation_set->at(ani)->Render(x, y);

	for (LPGAMEOBJECT fireball : listFireball)
		fireball->Render();
}

void CFirePiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (piranhaType == TypeOfFirePiranha::RED)
	{
		l = x;
		t = y;
		r = x + RED_FIRE_PIRANHA_BBOX_WIDTH;
		b = y + RED_FIRE_PIRANHA_BBOX_HEIGHT;
	}
	else if (piranhaType == TypeOfFirePiranha::GREEN)
	{
		l = x;
		t = y;
		r = x + GREEN_FIRE_PIRANHA_BBOX_WIDTH;
		b = y + GREEN_FIRE_PIRANHA_BBOX_HEIGHT;
	}
}

void CFirePiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PIRANHA_STATE_MOVE_UP:
		vy = -FIRE_PIRANHA_MOVE_SPEED_Y;
		break;
	case FIRE_PIRANHA_STATE_MOVE_DOWN:
		vy = FIRE_PIRANHA_MOVE_SPEED_Y;
		break;
	case FIRE_PIRANHA_STATE_ATTACK: // this line is just for drawing
		break;
	case ENEMY_STATE_DIE_BY_WEAPON:
		isFinishedUsing = true;
		break;
	}
}

Area CFirePiranha::GetCurrentPlayerArea()
{
	player->GetBoundingBox(playerLeft, playerTop, playerRight, playerBottom);

	float horizontalSeparationLine = y + RED_FIRE_PIRANHA_BBOX_HEIGHT - 1;

	if (playerBottom < horizontalSeparationLine && playerBottom >= 200)
	{
		if (playerRight >= farLeftStart && playerRight < nearLeftStart)
			return Area::TOP_LEFT_FAR;
		else if (playerRight >= nearLeftStart && playerRight < nearRightStart)
			return Area::TOP_LEFT_NEAR;
		else if (playerRight >= nearRightStart && playerRight < farRightStart)
			return Area::TOP_RIGHT_NEAR;
		else if (playerRight >= farRightStart && playerRight <= farRightEnd)
			return Area::TOP_RIGHT_FAR;
	}
	else if (playerBottom >= horizontalSeparationLine) // 367
	{
		if (playerRight >= farLeftStart && playerRight < nearLeftStart)
			return Area::BOTTOM_LEFT_FAR;
		else if (playerRight >= nearLeftStart && playerRight < nearRightStart)
			return Area::BOTTOM_LEFT_NEAR;
		else if (playerRight >= nearRightStart && playerRight < farRightStart)
			return Area::BOTTOM_RIGHT_NEAR;
		else if (playerRight >= farRightStart && playerRight <= farRightEnd)
			return Area::BOTTOM_RIGHT_FAR;
	}
	return Area::OUTSIDE_AREA;
}

void CFirePiranha::CreateFireball()
{
	CPiranhaFireball* fireball = new CPiranhaFireball({ x, y }, playerArea, player);
	listFireball.push_back(fireball);
}

void CFirePiranha::SetAreaLimit()
{
	if (piranhaType == TypeOfFirePiranha::RED)
	{
		minPosY = RED_FIRE_PIRANHA_MIN_Y;
		farLeftStart = RED_FIRE_PIRANHA_FAR_LEFT_START;
		nearLeftStart = RED_FIRE_PIRANHA_NEAR_LEFT_START;
		nearRightStart = RED_FIRE_PIRANHA_NEAR_RIGHT_START;
		farRightStart = RED_FIRE_PIRANHA_FAR_RIGHT_START;
		farRightEnd = RED_FIRE_PIRANHA_FAR_RIGHT_END;
		safeZoneLeft = RED_FIRE_PIRANHA_SAFE_ZONE_LEFT;
		safeZoneRight = RED_FIRE_PIRANHA_SAFE_ZONE_RIGHT;
		safeZoneBottom = RED_FIRE_PIRANHA_SAFE_ZONE_BOTTOM;
	}
	else
	{
		minPosY = GREEN_FIRE_PIRANHA_MIN_Y;
		farLeftStart = GREEN_FIRE_PIRANHA_FAR_LEFT_START;
		nearLeftStart = GREEN_FIRE_PIRANHA_NEAR_LEFT_START;
		nearRightStart = GREEN_FIRE_PIRANHA_NEAR_RIGHT_START;
		farRightStart = GREEN_FIRE_PIRANHA_FAR_RIGHT_START;
		farRightEnd = GREEN_FIRE_PIRANHA_FAR_RIGHT_END;
		safeZoneLeft = GREEN_FIRE_PIRANHA_SAFE_ZONE_LEFT;
		safeZoneRight = GREEN_FIRE_PIRANHA_SAFE_ZONE_RIGHT;
		safeZoneBottom = GREEN_FIRE_PIRANHA_SAFE_ZONE_BOTTOM;
	}
}

bool CFirePiranha::CheckPlayerInSafeZone(float pl, float pt, float pr, float pb)
{
	return (pl < safeZoneRight
		&& pr > safeZoneLeft
		&& pt < safeZoneBottom
		&& pb > 0);
}

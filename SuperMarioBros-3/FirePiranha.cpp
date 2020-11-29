#include "FirePiranha.h"

CFirePiranha::CFirePiranha(CMario* mario, int piranhaType)
{
	category = Category::ENEMY;
	player = mario;
	this->piranhaType = piranhaType;

	SetAreaLimit();
	SetState(FIRE_PIRANHA_STATE_MOVE_UP);
}

void CFirePiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	if (effect)
		effect->Update(dt, coObjects);

	if (deadTime->IsTimeUp())
		isFinishedUsing = true;

	if (vanish)
	{
		for (size_t i = 0; i < listFireball.size(); i++)
			listFireball.erase(listFireball.begin() + i);
		return;
	}

	for (size_t i = 0; i < listFireball.size(); i++)
	{
		if (listFireball[i]->isFinishedUsing)
			listFireball.erase(listFireball.begin() + i);
	}

	if (delayToAttackTime->IsTimeUp())
	{
		if (playerArea != Area::OUTSIDE_AREA)
			CreateFireball();
		delayToAttackTime->Stop();
	}

	for (LPGAMEOBJECT fireball : listFireball)
		fireball->Update(dt, coObjects);

	if (attackTime->IsStopped() && y <= minPosY)
	{
		y = minPosY;
		vy = 0;
		attackTime->Start();
		delayToAttackTime->Start();
		SetState(FIRE_PIRANHA_STATE_ATTACK);
	}
	else if (sleepTime->IsStopped() && y >= FIRE_PIRANHA_MAX_Y)
	{
		y = FIRE_PIRANHA_MAX_Y;
		vy = 0;
		sleepTime->Start();
	}

	if (attackTime->IsTimeUp())
	{
		attackTime->Stop();
		SetState(FIRE_PIRANHA_STATE_MOVE_DOWN);
	}

	if (sleepTime->IsTimeUp())
	{
		if (!CheckPlayerInSafeZone(playerLeft, playerTop, playerRight, playerBottom))
		{
			sleepTime->Stop();
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
		case Area::TOP_LEFT_FAR:
		case Area::TOP_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_UP_LEFT;
			break;
		case Area::TOP_RIGHT_FAR:
		case Area::TOP_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_UP_RIGHT;
			break;
		case Area::BOTTOM_LEFT_FAR:
		case Area::BOTTOM_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;
			break;
		case Area::BOTTOM_RIGHT_FAR:
		case Area::BOTTOM_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_RIGHT;
			break;
		case Area::OUTSIDE_AREA:
			ani = last_attack_ani;
			break;
		}
		last_attack_ani = ani;
	}
	else
	{
		switch (playerArea)
		{
		case Area::TOP_LEFT_FAR:
		case Area::TOP_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_UP_LEFT;
			break;
		case Area::TOP_RIGHT_FAR:
		case Area::TOP_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_UP_RIGHT;
			break;
		case Area::BOTTOM_LEFT_FAR:
		case Area::BOTTOM_LEFT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
			break;
		case Area::BOTTOM_RIGHT_FAR:
		case Area::BOTTOM_RIGHT_NEAR:
			ani = FIRE_PIRANHA_ANI_FACE_DOWN_RIGHT;
			break;
		case Area::OUTSIDE_AREA:
			ani = last_face_ani;
			break;
		}
		last_face_ani = ani;
	}

	if (!vanish)
		animation_set->at(ani)->Render(x, y);

	for (LPGAMEOBJECT fireball : listFireball)
		fireball->Render();

	if (effect)
		effect->Render();

	//RenderBoundingBox();
}

void CFirePiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (vanish)
		return;

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
		effect = new CMoneyEffect({ x + 3, y - 7 });
		vanish = true;
		deadTime->Start();
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

#include "FirePiranha.h"

CFirePiranha::CFirePiranha(CMario* mario, int piranhaType)
{
	category = ENEMY;
	type = FIRE_PIRANHA;
	player = mario;
	this->piranhaType = piranhaType;
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
		DebugOut(L"SHOOT!\n");
		//if (listFireball.empty()) // để tạm test thôi, xong xóa
			CreateFireball();
		delayToAttackStartTime = 0;
	}

	for (LPGAMEOBJECT fireball : listFireball)
		fireball->Update(dt, coObjects);

	if (!attackStartTime && y <= RED_FIRE_PIRANHA_MIN_Y)
	{
		DebugOut(L"set state attack, attack time begin\n");
		y = RED_FIRE_PIRANHA_MIN_Y;
		vy = 0;
		attackStartTime = GetTickCount();
		delayToAttackStartTime = GetTickCount();
		//SetState(FIRE_PIRANHA_STATE_ATTACK);
	}
	else if (!sleepStartTime && y >= RED_FIRE_PIRANHA_MAX_Y)
	{
		DebugOut(L"sleep time begin\n");
		y = RED_FIRE_PIRANHA_MAX_Y;
		vy = 0;
		sleepStartTime = GetTickCount();
	}

	if (attackStartTime && GetTickCount() - attackStartTime > FIRE_PIRANHA_DELAY_TIME)
	{
		DebugOut(L"set state move down\n");
		attackStartTime = 0;
		SetState(FIRE_PIRANHA_STATE_MOVE_DOWN);
	}

	if (sleepStartTime && GetTickCount() - sleepStartTime > FIRE_PIRANHA_DELAY_TIME)
	{
		sleepStartTime = 0;
		SetState(FIRE_PIRANHA_STATE_MOVE_UP);
	}
}

void CFirePiranha::Render()
{
	playerArea = GetCurrentPlayerArea();

	if (piranhaType == TypeOfFirePiranha::RED)
	{
		if (state == FIRE_PIRANHA_STATE_ATTACK)
		{
			switch (playerArea)
			{
			case TOP_LEFT_FAR:
			case TOP_LEFT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_ATTACK_UP_LEFT;
				break;
			case TOP_RIGHT_FAR:
			case TOP_RIGHT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_ATTACK_UP_RIGHT;
				break;
			case BOTTOM_LEFT_FAR:
			case BOTTOM_LEFT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;
				break;
			case BOTTOM_RIGHT_FAR:
			case BOTTOM_RIGHT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_ATTACK_DOWN_RIGHT;
				break;
			default:
				ani = RED_FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;
				break;
			}
		}
		else
		{
			switch (playerArea)
			{
			case TOP_LEFT_FAR:
			case TOP_LEFT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_FACE_UP_LEFT;
				break;
			case TOP_RIGHT_FAR:
			case TOP_RIGHT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_FACE_UP_RIGHT;
				break;
			case BOTTOM_LEFT_FAR:
			case BOTTOM_LEFT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
				break;
			case BOTTOM_RIGHT_FAR:
			case BOTTOM_RIGHT_NEAR:
				ani = RED_FIRE_PIRANHA_ANI_FACE_DOWN_RIGHT;
				break;
			default:
				ani = RED_FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
				break;
			}
		}
	}

	else if (piranhaType == TypeOfFirePiranha::GREEN)
	{
		//
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
	case FIRE_PIRANHA_STATE_ATTACK:
		break;
	case ENEMY_STATE_DIE_BY_WEAPON:
		isFinishedUsing = true;
		break;
	}
}

Area CFirePiranha::GetCurrentPlayerArea()
{
	float playerPosX = player->x;
	float playerPosY = player->y; // chỗ này nghĩ là nên lấy bottom của mario

	if (playerPosY <= 367 && playerPosY >= 200)
	{
		if (playerPosX >= 248 && playerPosX <= 295)
			return Area::TOP_LEFT_FAR;
		else if (playerPosX >= 296 && playerPosX <= 367)
			return Area::TOP_LEFT_NEAR;
		else if (playerPosX >= 368 && playerPosX <= 439)
			return Area::TOP_RIGHT_NEAR;
		else if (playerPosX >= 440 && playerPosX <= 487)
			return Area::TOP_RIGHT_FAR;
	}
	else if (player->y >= 368)
	{
		if (playerPosX >= 248 && playerPosX <= 295)
			return Area::BOTTOM_LEFT_FAR;
		else if (playerPosX >= 296 && playerPosX <= 367)
			return Area::BOTTOM_LEFT_NEAR;
		else if (playerPosX >= 368 && playerPosX <= 439)
			return Area::BOTTOM_RIGHT_NEAR;
		else if (playerPosX >= 440 && playerPosX <= 487)
			return Area::BOTTOM_RIGHT_FAR;
	}
	return Area::OUTSIDE_AREA;
}

void CFirePiranha::CreateFireball()
{
	CPiranhaFireball* fireball = new CPiranhaFireball({ x, y }, playerArea);
	listFireball.push_back(fireball);
}

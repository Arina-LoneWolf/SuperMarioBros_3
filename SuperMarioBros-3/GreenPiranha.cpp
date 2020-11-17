#include "GreenPiranha.h"

CGreenPiranha::CGreenPiranha(CMario* mario)
{
	category = ENEMY;
	type = GREEN_PIRANHA;
	player = mario;
	SetState(FIRE_PIRANHA_STATE_MOVE_UP);
}

void CGreenPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	// bug hahahaha !!
	if (!attackStartTime && y <= GREEN_PIRANHA_MIN_Y)
	{
		y = GREEN_PIRANHA_MIN_Y;
		vy = 0;
		attackStartTime = GetTickCount();
	}
	else if (!sleepStartTime && y >= GREEN_PIRANHA_MAX_Y)
	{
		y = GREEN_PIRANHA_MAX_Y;
		vy = 0;
		sleepStartTime = GetTickCount();
	}

	if (attackStartTime && GetTickCount() - attackStartTime > GREEN_PIRANHA_DELAY_TIME)
	{
		attackStartTime = 0;
		SetState(GREEN_PIRANHA_STATE_MOVE_DOWN);
	}

	if (sleepStartTime && GetTickCount() - sleepStartTime > GREEN_PIRANHA_DELAY_TIME)
	{
		player->GetBoundingBox(playerLeft, playerTop, playerRight, playerBottom);

		if (!CheckPlayerInSafeZone(playerLeft, playerTop, playerRight, playerBottom))
		{
			sleepStartTime = 0;
			SetState(GREEN_PIRANHA_STATE_MOVE_UP);
		}
	}
}

void CGreenPiranha::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CGreenPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GREEN_PIRANHA_BBOX_WIDTH;
	b = y + GREEN_PIRANHA_BBOX_HEIGHT;
}

void CGreenPiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_PIRANHA_STATE_MOVE_UP:
		vy = -GREEN_PIRANHA_MOVE_SPEED_Y;
		break;
	case GREEN_PIRANHA_STATE_MOVE_DOWN:
		vy = GREEN_PIRANHA_MOVE_SPEED_Y;
		break;
	case ENEMY_STATE_DIE_BY_WEAPON:
		isFinishedUsing = true;
		break;
	}
}

bool CGreenPiranha::CheckPlayerInSafeZone(float pl, float pt, float pr, float pb)
{
	return (pl < GREEN_PIRANHA_SAFE_ZONE_RIGHT
		&& pr > GREEN_PIRANHA_SAFE_ZONE_LEFT
		&& pt < GREEN_PIRANHA_SAFE_ZONE_BOTTOM
		&& pb > 0);
}

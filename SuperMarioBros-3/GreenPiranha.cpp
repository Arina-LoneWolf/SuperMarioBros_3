#include "GreenPiranha.h"

CGreenPiranha::CGreenPiranha(CMario* mario)
{
	category = Category::ENEMY;
	type = Type::GREEN_PIRANHA;
	player = mario;
	SetState(FIRE_PIRANHA_STATE_MOVE_UP);
}

void CGreenPiranha::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	
	if (effect)
		effect->Update(dt, coObjects);

	if (deadTime->IsTimeUp())
		isFinishedUsing = true;

	if (attackTime->IsStopped() && y <= GREEN_PIRANHA_MIN_Y)
	{
		y = GREEN_PIRANHA_MIN_Y;
		vy = 0;
		attackTime->Start();
	}
	else if (sleepTime->IsStopped() && y >= GREEN_PIRANHA_MAX_Y)
	{
		y = GREEN_PIRANHA_MAX_Y;
		vy = 0;
		sleepTime->Start();
	}

	if (attackTime->IsTimeUp())
	{
		attackTime->Stop();
		SetState(GREEN_PIRANHA_STATE_MOVE_DOWN);
	}

	if (sleepTime->IsTimeUp())
	{
		player->GetBoundingBox(playerLeft, playerTop, playerRight, playerBottom);

		if (!CheckPlayerInSafeZone(playerLeft, playerTop, playerRight, playerBottom))
		{
			sleepTime->Stop();
			SetState(GREEN_PIRANHA_STATE_MOVE_UP);
		}
	}
}

void CGreenPiranha::Render()
{
	if (!vanish)
		animation_set->at(0)->Render(x, y);

	if (effect)
		effect->Render();

	//RenderBoundingBox();
}

void CGreenPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (vanish)
		return;

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
		effect = new CMoneyEffect({ x + 3, y - 7 });
		vanish = true;
		deadTime->Start();
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

#include "PiranhaFireball.h"

CPiranhaFireball::CPiranhaFireball(D3DXVECTOR2 piranhaPos, Area playerArea, CMario* player)
{
	// đọc player area và set vị trí, hướng bay
	this->x = piranhaPos.x + 4;
	this->y = piranhaPos.y + 4;
	this->player = player;

	SetRoute(playerArea);
	SetNx(playerArea);

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void CPiranhaFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2 || y < CGame::GetInstance()->GetCamPosY())
		isFinishedUsing = true;

	if (listEffect.empty())
	{
		x += dx;
		y += dy;
	}

	for (LPGAMEOBJECT effect : listEffect)
	{
		if (effect->isFinishedUsing)
			listEffect.erase(listEffect.begin());
	}

	for (LPGAMEOBJECT effect : listEffect)
		effect->Update(dt, coObjects);

	float ml, mt, mr, mb, pl, pt, pr, pb; // main object - fireball (m) and the player (p)
	GetBoundingBox(ml, mt, mr, mb);
	player->GetBoundingBox(pl, pt, pr, pb);

	if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
	{
		if (!player->GetUntouchable())
		{
			CreateEffect();
			player->CollideWithEnemy();
		}
	}
}

void CPiranhaFireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIREBALL_BBOX_WIDTH;
	b = y + FIREBALL_BBOX_HEIGHT;
}

void CPiranhaFireball::Render()
{
	if (nx > 0)
		animation_set->at(FIREBALL_TO_RIGHT)->Render(x, y);
	else
		animation_set->at(FIREBALL_TO_LEFT)->Render(x, y);

	for (LPGAMEOBJECT effect : listEffect)
		effect->Render();

	//RenderBoundingBox();
}

void CPiranhaFireball::SetRoute(Area playerArea)
{
	switch (playerArea)
	{
	case Area::TOP_LEFT_FAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = vx / 2.8f;
		break;
	case Area::TOP_LEFT_NEAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = vx;
		break;
	case Area::TOP_RIGHT_FAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = -vx / 2.8f;
		break;
	case Area::TOP_RIGHT_NEAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = -vx;
		break;
	case Area::BOTTOM_LEFT_FAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = -vx / 2.8f;
		break;
	case Area::BOTTOM_LEFT_NEAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = -vx;
		break;
	case Area::BOTTOM_RIGHT_FAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = vx / 2.8f;
		break;
	case Area::BOTTOM_RIGHT_NEAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = vx;
		break;
	}
}

void CPiranhaFireball::SetNx(Area playerArea)
{
	switch (playerArea)
	{
	case Area::TOP_LEFT_FAR:
	case Area::TOP_LEFT_NEAR:
	case Area::BOTTOM_LEFT_FAR:
	case Area::BOTTOM_LEFT_NEAR:
		nx = -1;
		break;
	case Area::TOP_RIGHT_FAR:
	case Area::TOP_RIGHT_NEAR:
	case Area::BOTTOM_RIGHT_FAR:
	case Area::BOTTOM_RIGHT_NEAR:
		nx = 1;
		break;
	}
}

void CPiranhaFireball::CreateEffect()
{
	CHitEffect* effect = new CHitEffect({ x, y }, nx);
	listEffect.push_back(effect);
}

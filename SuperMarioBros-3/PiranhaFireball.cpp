#include "PiranhaFireball.h"

CPiranhaFireball::CPiranhaFireball(D3DXVECTOR2 piranhaPos, Area playerArea)
{
	// đọc player area và set vị trí, hướng bay
	this->x = piranhaPos.x + 4;
	this->y = piranhaPos.y + 4;
	SetRoute(playerArea);
	SetNx(playerArea);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void CPiranhaFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2 || y < CGame::GetInstance()->GetCamPosY())
		isFinishedUsing = true;

	if (collisionStartTime && GetTickCount() - collisionStartTime > 500)
	{
		collisionIsDone = true;
	}

	if (!collisionStartTime || collisionIsDone)
	{
		x += dx;
		y += dy;
	}

	float ml, mt, mr, mb, ol, ot, or, ob; // main object (m) and the other one (o)
	GetBoundingBox(ml, mt, mr, mb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ol, ot, or , ob);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, ol, ot, or, ob))
		{
			DebugOut(L"fireball collide, object type is %d\n", e->type);
			if (e->category == Type::MARIO /*&& !collisionIsDone*/)
			{
				// viên đạn đứng yên trong giây lát
				collisionStartTime = GetTickCount();
				CMario* mario = dynamic_cast<CMario*>(e);
				mario->CollideWithEnemy();
			}
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

	//RenderBoundingBox();
}

void CPiranhaFireball::SetRoute(Area playerArea)
{
	switch (playerArea)
	{
	case TOP_LEFT_FAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = vx / 2.8;
		break;
	case TOP_LEFT_NEAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = vx;
		break;
	case TOP_RIGHT_FAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = -vx / 2.8;
		break;
	case TOP_RIGHT_NEAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = -vx;
		break;
	case BOTTOM_LEFT_FAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = -vx / 2.8;
		break;
	case BOTTOM_LEFT_NEAR:
		vx = -PIRANHA_FIREBALL_SPEED_X;
		vy = -vx;
		break;
	case BOTTOM_RIGHT_FAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = vx / 2.8;
		break;
	case BOTTOM_RIGHT_NEAR:
		vx = PIRANHA_FIREBALL_SPEED_X;
		vy = vx;
		break;
	}
}

void CPiranhaFireball::SetNx(Area playerArea)
{
	switch (playerArea)
	{
	case TOP_LEFT_FAR:
	case TOP_LEFT_NEAR:
	case BOTTOM_LEFT_FAR:
	case BOTTOM_LEFT_NEAR:
		nx = -1;
		break;
	case TOP_RIGHT_FAR:
	case TOP_RIGHT_NEAR:
	case BOTTOM_RIGHT_FAR:
	case BOTTOM_RIGHT_NEAR:
		nx = 1;
		break;
	}
}

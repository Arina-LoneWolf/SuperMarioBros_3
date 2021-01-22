#include "FloatingWood.h"

CFloatingWood::CFloatingWood()
{
	category = Category::MISC;
}

void CFloatingWood::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR >= this->x && !isFloating)
	{
		SetState(STATE_FLOATING);
		isFloating = true;
	}
}

void CFloatingWood::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void CFloatingWood::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + FLOATING_WOOD_BBOX_WIDTH;
	b = t + FLOATING_WOOD_BBOX_HEIGHT;
}

void CFloatingWood::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_FLOATING)
	{
		vx = -FLOATING_WOOD_SPEED_X;
	}
	else if (state == STATE_SINKING)
	{
		vx = 0;
		vy = FLOATING_WOOD_SPEED_Y;
	}
}

CFloatingWood::~CFloatingWood()
{
}

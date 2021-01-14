#include "BoomerangBrother.h"

CBoomerangBrother::CBoomerangBrother()
{
	category = Category::ENEMY;
}

void CBoomerangBrother::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CBoomerangBrother::Render()
{
}

void CBoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BOOMERANG_BROTHER_BBOX_WIDTH;
	b = t + BOOMERANG_BROTHER_BBOX_HEIGHT;
}

void CBoomerangBrother::SetState(int state)
{
}

void CBoomerangBrother::CreateBoomerang()
{
	CBoomerang* boomerang = new CBoomerang({ x, y }, nx);
	listBoomerang.push_back(boomerang);
}

CBoomerangBrother::~CBoomerangBrother()
{
}

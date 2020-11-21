#include "IceFlower.h"

CIceFlower::CIceFlower(float brickX, float brickY)
{
	type = ICE_FLOWER;
	category = ITEM;

	this->x = brickX;
	this->y = brickY;
	vy = -ICE_FLOWER_DEFLECT_SPEED_Y;

	minPosY = brickY - ICE_FLOWER_BBOX_WIDTH;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(6));
}

void CIceFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy;

	if (y <= minPosY)
	{
		vy = 0;

		if (!existingTime)
			existingTime = GetTickCount64();
	}
	DebugOut(L"existing time: %d\n", GetTickCount64() - existingTime);
	if (existingTime && GetTickCount64() - existingTime > ICE_FLOWER_MAX_EXISTING_TIME)
		isFinishedUsing = true;
}

void CIceFlower::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CIceFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ICE_FLOWER_BBOX_WIDTH;
	b = y + ICE_FLOWER_BBOX_HEIGHT;
}

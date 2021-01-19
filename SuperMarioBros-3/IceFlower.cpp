#include "IceFlower.h"

CIceFlower::CIceFlower(float brickX, float brickY)
{
	type = Type::ICE_FLOWER;
	category = Category::ITEM;

	this->x = brickX;
	this->y = brickY;
	vy = -ICE_FLOWER_DEFLECT_SPEED_Y;

	minPosY = brickY - ICE_FLOWER_BBOX_WIDTH;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(6));
}

void CIceFlower::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy;

	if (y <= minPosY)
		vy = 0;
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

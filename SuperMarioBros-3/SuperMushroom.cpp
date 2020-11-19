#include "SuperMushroom.h"

CSuperMushroom::CSuperMushroom(float brickX, float brickY)
{
	type = SUPER_MUSHROOM;
	category = ITEM;
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CSuperMushroom::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CSuperMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}

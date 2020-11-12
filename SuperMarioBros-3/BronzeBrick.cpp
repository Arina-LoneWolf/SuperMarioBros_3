#include "BronzeBrick.h"

CBronzeBrick::CBronzeBrick()
{
	type = BRONZE_BRICK;
	category = MISC;
}

void CBronzeBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBronzeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIDE_LENGTH;
	b = y + BRICK_BBOX_SIDE_LENGTH;
}

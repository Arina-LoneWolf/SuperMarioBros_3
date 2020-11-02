#include "GroundTile.h"

void CGroundTile::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CGroundTile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GROUNDTILE_BBOX_WIDTH;
	b = y + GROUNDTILE_BBOX_HEIGHT;
}
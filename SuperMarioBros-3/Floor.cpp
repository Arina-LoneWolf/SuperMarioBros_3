#include "Floor.h"

CFloor::CFloor(int width_in_units, int height_in_units)
{
	type = FLOOR;
	category = MISC;
	this->width = width_in_units * TILESET_BBOX_SIDE;
	this->height = height_in_units * TILESET_BBOX_SIDE;
}

void CFloor::Render()
{
	//RenderBoundingBox();
}

void CFloor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
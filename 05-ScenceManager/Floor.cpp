#include "Floor.h"

CFloor::CFloor(int width, int height)
{
	type = FLOOR;
	category = MISC;
	this->width = width;
	this->height = height;
}

void CFloor::Render()
{
	//RenderBoundingBox();
}

void CFloor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBOX_WIDTH * width;
	b = y + BBOX_HEIGHT * height;
}
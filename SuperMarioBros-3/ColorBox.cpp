#include "ColorBox.h"


CColorBox::CColorBox(int width_in_units, int height_in_units) : CGameObject::CGameObject()
{
	category = Category::MISC;
	this->width = width_in_units * TILESET_BBOX_SIDE_LENGTH;
	this->height = height_in_units * TILESET_BBOX_SIDE_LENGTH;
}

void CColorBox::Render()
{
	//RenderBoundingBox();
}

void CColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	type = PORTAL;
	category = SCENE;
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
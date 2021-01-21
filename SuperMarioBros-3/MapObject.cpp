#include "MapObject.h"

CMapObject::CMapObject()
{
	vx = -GATEKEEPER_SPEED_X;
	vx = -0.007f;
}

void CMapObject::Render()
{
	if (type == Type::TUSSOCK)
		ani = MAP_OBJECT_ANI_TUSSOCK;
	else if (type == Type::HELP_BUBBLE)
		ani = MAP_OBJECT_ANI_HELP_BUBBLE;
	else
	{
		if (vx > 0)
			ani = MAP_OBJECT_ANI_GATEKEEPER_RIGHT;
		else
			ani = MAP_OBJECT_ANI_GATEKEEPER_LEFT;
	}

	animation_set->at(ani)->Render(x, y);
}

void CMapObject::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (type == Type::GATEKEEPER)
	{
		CGameObject::Update(dt, coObjects);

		x += dx;

		if ((x <= GATEKEEPER_LEFT_LIMIT && vx < 0) || (x >= GATEKEEPER_RIGHT_LIMIT && vx > 0))
			vx = -vx;
	}
}

void CMapObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

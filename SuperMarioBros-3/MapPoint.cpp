#include "MapPoint.h"

CMapPoint::CMapPoint(float x, float y, int sceneID, bool left, bool right, bool above, bool under)
{
	this->x = x;
	this->y = y;
	leftEdge = x;
	bottomEdge = y + 16;
	this->sceneID = sceneID;
	hasPoint.insert(hasPoint.end(), { left, right, above, under });
}

void CMapPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 6;
	t = y + 6;
	r = l + POINT_SIDE_LENGTH;
	b = t + POINT_SIDE_LENGTH;
}

void CMapPoint::Render()
{
	if (isConquered)
		animation_set->at(0)->Render(x, y);
	
	//RenderBoundingBox();
}

CMapPoint::~CMapPoint()
{
}

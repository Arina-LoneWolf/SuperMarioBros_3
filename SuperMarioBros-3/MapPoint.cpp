#include "MapPoint.h"

//int id_count = 0;
//int get_id()
//{
//	static int id = id_count++;
//	return id;
//}

CMapPoint::CMapPoint(float x, float y, int sceneID, bool left, bool right, bool above, bool under, int id)
{
	this->id = id;
	this->x = x;
	this->y = y;
	leftEdge = x;
	topEdge = y;
	rightEdge = x + 16;
	bottomEdge = y + 16;
	this->sceneID = sceneID;
	hasPointAround.insert(hasPointAround.end(), { left, right, above, under });
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

#include "MapPoint.h"

CMapPoint::CMapPoint(float x, float y, int sceneID, bool left, bool right, bool above, bool under)
{
	this->x = x;
	this->y = y;
	leftEdge = x;
	bottomEdge = y + 16;
	this->sceneID = sceneID;
	//hasPoint.insert(hasPoint.end(), { left, right, above, under });

	this->left = left;
	this->right = right;
	this->above = above;
	this->under = under;
}

void CMapPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 6;
	t = y + 6;
	r = l + POINT_SIDE_LENGTH;
	b = t + POINT_SIDE_LENGTH;
}

//bool operator==(CMapPoint p1, CMapPoint p2)
//{
//	DebugOut(L"aaaa");
//	return (p1.leftEdge == p2.leftEdge) && (p1.bottomEdge == p2.bottomEdge);
//}

void CMapPoint::Render()
{
	RenderBoundingBox();
}

CMapPoint::~CMapPoint()
{
}

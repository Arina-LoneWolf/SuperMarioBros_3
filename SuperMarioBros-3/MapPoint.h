#pragma once
#include "GameObject.h"

class CMapPoint : public CGameObject
{
public:
	float leftEdge, topEdge, rightEdge, bottomEdge;
	int sceneID;
	int id;
	vector<bool> hasPointAround;
	bool isConquered;

	CMapPoint(float x, float y, int sceneID, bool left, bool right, bool above, bool under, int id);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	bool HasPortal() { return !sceneID; }
	~CMapPoint();
};


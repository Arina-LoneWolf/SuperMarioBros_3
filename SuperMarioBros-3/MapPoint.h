#pragma once
#include "GameObject.h"

class CMapPoint : public CGameObject
{
public:
	float leftEdge, bottomEdge;
	int sceneID;
	vector<bool> hasPoint;

	CMapPoint(float x, float y, int sceneID, bool left, bool right, bool above, bool under);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	bool HasPortal() { return !sceneID; }
	~CMapPoint();
};


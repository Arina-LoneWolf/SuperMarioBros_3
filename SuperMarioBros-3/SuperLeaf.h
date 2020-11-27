#pragma once
#include "GameObject.h"
#include "Math.h"

class CSuperLeaf : public CGameObject
{
public:
	float minPosY;
	float leftLimit, rightLimit;
	bool isFalling;

	CSuperLeaf(float brickX, float brickY);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


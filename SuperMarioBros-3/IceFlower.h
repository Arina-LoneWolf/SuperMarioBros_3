#pragma once
#include "GameObject.h"

class CIceFlower : public CGameObject
{
public:
	ULONGLONG existingTime;
	float minPosY;

	CIceFlower(float brickX, float brickY);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


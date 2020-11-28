#pragma once
#include "GameObject.h"
#include "Timer.h"

class CIceFlower : public CGameObject
{
public:
	CTimer* existingTime = new CTimer(ICE_FLOWER_MAX_EXISTING_TIME);
	float minPosY;

	CIceFlower(float brickX, float brickY);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


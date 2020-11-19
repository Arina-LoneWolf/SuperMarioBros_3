#pragma once
#include "GameObject.h"

class CSuperMushroom : public CGameObject
{
	float minPosY;
	float leftLimit, rightLimit;
	bool isFalling;

	CSuperMushroom(float brickX, float brickY);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


#pragma once
#include "GameObject.h"

class CSuperMushroom : public CGameObject
{
public:
	float minPosY;
	bool stopBouncing;
	bool isSliding;

	CSuperMushroom(float brickX, float brickY);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


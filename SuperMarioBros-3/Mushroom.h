#pragma once
#include "GameObject.h"

class CMushroom : public CGameObject
{
public:
	int mushroomType;
	float minPosY;
	bool stopBouncing = false;
	bool isSliding = false;

	CMushroom(float brickX, float brickY, int mushroomType);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


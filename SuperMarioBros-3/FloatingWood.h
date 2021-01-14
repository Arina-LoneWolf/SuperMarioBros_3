#pragma once
#include "GameObject.h"

class CFloatingWood : public CGameObject
{
public:
	bool isFloating = false;

	CFloatingWood();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};


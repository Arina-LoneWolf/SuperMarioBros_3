#pragma once
#include "GameObject.h"

class CTail : public CGameObject
{
public:
	bool appear;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};


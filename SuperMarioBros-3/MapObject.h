#pragma once
#include "GameObject.h"

class CMapObject : public CGameObject
{
public:
	CMapObject();
	virtual void Render();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


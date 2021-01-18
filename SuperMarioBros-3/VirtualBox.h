#pragma once
#include "GameObject.h"

class CVirtualBox : public CGameObject
{
public:
	bool dropped;

	CVirtualBox();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


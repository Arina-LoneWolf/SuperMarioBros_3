#pragma once
#include "GameObject.h"
#include "Boomerang.h"

class CBoomerangBrother : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listBoomerang;

	CBoomerangBrother();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void CreateBoomerang();
	~CBoomerangBrother();
};


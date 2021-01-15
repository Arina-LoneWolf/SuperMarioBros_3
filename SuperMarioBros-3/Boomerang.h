#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "HitEffect.h"

class CBoomerang : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listEffect;
	CTimer* delayToHurl = new CTimer(250);

	CBoomerang(D3DXVECTOR2 position, int nx);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	void CreateEffect();
	~CBoomerang();
};


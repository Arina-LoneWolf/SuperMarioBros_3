#pragma once
#include "GameObject.h"
#include "BronzeBrick.h"
#include "TailHitEffect.h"

class CTail : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listEffect;
	bool appear;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();

	void CreateEffect();
};


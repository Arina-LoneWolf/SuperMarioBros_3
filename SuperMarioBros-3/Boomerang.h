#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "HitEffect.h"

class CBoomerang : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listEffect;
	CTimer* delayToHurl = new CTimer(BOOMERANG_HURLING_DELAY_TIME);
	bool directionChanged;
	float initialPosX;
	float limitPosY;

	CBoomerang(int nx);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	virtual void SetState(int state);
	void CreateEffect();
	void SetPositionFollowBoomerangBrother();
	~CBoomerang();
};


#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "MoneyEffect.h"

class CGreenPiranha : public CGameObject
{
public:
	CTimer* attackTime = new CTimer(GREEN_PIRANHA_DELAY_TIME);
	CTimer* sleepTime = new CTimer(GREEN_PIRANHA_DELAY_TIME);
	CTimer* deadTime = new CTimer(PIRANHA_MAX_EXISTING_TIME_AFTER_DEATH);

	float playerLeft, playerTop, playerRight, playerBottom;
	bool vanish = false;

	CMario* player;
	CMoneyEffect* effect;

	CGreenPiranha(CMario* mario);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	bool CheckPlayerInSafeZone(float pl, float pt, float pr, float pb);
};


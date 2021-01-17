#pragma once
#include "GameObject.h"
#include "MoneyEffect.h"
#include "Mario.h"
//#include "Timer.h"

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	
public:
	CTimer* deadTime = new CTimer(GOOMBA_MAX_EXISTING_TIME_AFTER_DEATH);
	CTimer* walkTime = new CTimer(800);
	bool died = false;
	int lowFlyingCounter;

	CMoneyEffect* effect;
	CMario* player;

	CGoomba(CMario* player);
	virtual void SetState(int state);
};
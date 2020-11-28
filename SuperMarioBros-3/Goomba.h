#pragma once
#include "GameObject.h"
#include "MoneyEffect.h"
#include "Timer.h"

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	
public:
	CTimer* deadTime = new CTimer(GOOMBA_MAX_EXISTING_TIME_AFTER_DEATH);
	bool died = false;

	CMoneyEffect* effect;

	CGoomba();
	virtual void SetState(int state);
};
#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "MoneyEffect.h"

class CRedKoopa : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool died;
	bool isSupine;
	bool isBeingHeld;

	int startingPos;
	
	CMario* player;
	CMoneyEffect* effect;
	
	CRedKoopa(CMario* mario, int startingPos);
	virtual void SetState(int state);
	void SetPositionAccordingToPlayer();
	void Reset();
};
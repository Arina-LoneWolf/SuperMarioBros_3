#pragma once
#include "Math.h"
#include "GameObject.h"
#include "Mario.h"
#include "MoneyEffect.h"

class CKoopa : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool died;
	bool isSupine;
	bool isBeingHeld;
	bool reset;

	ULONGLONG sleepStartTime;
	ULONGLONG vibrationStartTime;

	float lastMoveSpeed = KOOPA_MOVE_SPEED_X;
	//float last_vx_para = GREEN_PARAKOOPA_MOVE_SPEED_X;
	int startingPos;
	
	CMario* player;
	CMoneyEffect* effect;
	
	CKoopa(CMario* mario, int startingPos, int koopaType);
	virtual void SetState(int state);
	void SetPositionAccordingToPlayer();
	void Reset();
	void Revive();
};
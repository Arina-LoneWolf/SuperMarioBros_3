#pragma once
#include "Math.h"
#include "GameObject.h"
#include "Mario.h"
#include "MoneyEffect.h"

class CKoopa : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool died;
	bool isSupine;
	bool isBeingHeld;
	bool reset;

	CTimer* sleepTime = new CTimer(KOOPA_SLEEP_TIME);
	CTimer* vibrationTime = new CTimer(KOOPA_VIBRATION_TIME);
	vector<LPGAMEOBJECT>* listBricks;

	float lastMoveSpeed = KOOPA_MOVE_SPEED_X;
	float startingPosX, startingPosY;
	
	CMario* player;
	CMoneyEffect* effect;
	
	CKoopa(CMario* mario, float x, float y, vector<LPGAMEOBJECT>* listBricks);
	virtual void SetState(int state);
	void SetPositionAccordingToPlayer();
	void Reset();
	void CreateEffect(float x, float y);
	bool DetectEdge(vector<LPGAMEOBJECT>* listBricks);
	void GetLeftBottomBBOX(float &l, float &b);
};
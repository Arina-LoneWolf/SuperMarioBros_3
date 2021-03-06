#pragma once
#include "Math.h"
#include "GameObject.h"
#include "Mario.h"
#include "ScoreEffect.h"
#include "VirtualBox.h"

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
	bool onBrick;
	bool justLostWings;
	Type initialType;

	CTimer* sleepTime = new CTimer(KOOPA_SLEEP_TIME);
	CTimer* vibrationTime = new CTimer(KOOPA_VIBRATION_TIME);

	float lastMoveSpeed = KOOPA_MOVE_SPEED_X;
	float startingPosX, startingPosY;
	
	CMario* player;
	CScoreEffect* effect;
	CVirtualBox* virtualBoxBelow = new CVirtualBox();
	CVirtualBox* virtualBoxHorizontally = new CVirtualBox();
	
	CKoopa(CMario* mario, float x, float y, Type type);
	virtual void SetState(int state);
	void SetPositionAccordingToPlayer();
	void Reset();
	void MakeEffectWhenSteppedOn();
	~CKoopa();
};
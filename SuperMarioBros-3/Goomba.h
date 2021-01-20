#pragma once
#include "GameObject.h"
#include "ScoreEffect.h"
#include "Mario.h"
//#include "Timer.h"

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	
public:
	CTimer* deadTime = new CTimer(GOOMBA_MAX_EXISTING_TIME_AFTER_DEATH);
	CTimer* walkTime = new CTimer(GOOMBA_WALK_TIME);
	CTimer* delayToRedirectAgain = new CTimer(GOOMBA_REDIRECTION_DELAY_TIME);
	CTimer* interestedInChasing = new CTimer(GOOMBA_INTERESTING_TIME);
	bool died = false;
	int lowFlyingCounter;
	bool lostWings;
	CScoreEffect* effect;
	CMario* player;

	CGoomba(CMario* player, Type goombaType);
	float GetSpeedX();
	virtual void SetState(int state);
};
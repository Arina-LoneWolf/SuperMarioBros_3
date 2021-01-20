#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PiranhaFireball.h"
#include "ScoreEffect.h"
#include "Timer.h"

class CFirePiranha : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listFireball;

	CTimer* attackTime = new CTimer(FIRE_PIRANHA_DELAY_TIME);
	CTimer* sleepTime = new CTimer(FIRE_PIRANHA_DELAY_TIME);
	CTimer* delayToAttackTime = new CTimer(FIRE_PIRANHA_DELAY_TO_ATTACK_TIME);
	CTimer* deadTime = new CTimer(PIRANHA_MAX_EXISTING_TIME_AFTER_DEATH);

	int last_face_ani = FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
	int last_attack_ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;

	float playerLeft, playerTop, playerRight, playerBottom;
	float farLeftStart, nearLeftStart, nearRightStart, farRightStart, farRightEnd;
	float safeZoneLeft, safeZoneRight, safeZoneBottom;

	int piranhaType;
	Area playerArea;
	CMario* player;
	CScoreEffect* effect;
	float minPosY;
	bool vanish = false;

	CFirePiranha(CMario* mario, int piranhaType);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

	Area GetCurrentPlayerArea();
	void CreateFireball();
	void SetAreaLimit();
	bool CheckPlayerInSafeZone(float pl, float pt, float pr, float pb);
};


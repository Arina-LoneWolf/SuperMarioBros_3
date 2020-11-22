#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PiranhaFireball.h"
#include "MoneyEffect.h"

class CFirePiranha : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listFireball;

	DWORD attackStartTime;
	DWORD sleepStartTime;
	DWORD delayToAttackStartTime;
	DWORD deadTime;

	int last_face_ani = FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;
	int last_attack_ani = FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;

	float playerLeft, playerTop, playerRight, playerBottom;
	float farLeftStart, nearLeftStart, nearRightStart, farRightStart, farRightEnd;
	float safeZoneLeft, safeZoneRight, safeZoneBottom;

	int piranhaType;
	Area playerArea;
	CMario* player;
	CMoneyEffect* effect;
	float minPosY;
	bool vanish;

	CFirePiranha(CMario* mario, int piranhaType);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

	Area GetCurrentPlayerArea();
	void CreateFireball();
	void SetAreaLimit();
	bool CheckPlayerInSafeZone(float pl, float pt, float pr, float pb);
};


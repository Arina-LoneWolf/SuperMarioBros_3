#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PiranhaFireball.h"

class CFirePiranha : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listFireball;

	DWORD attackStartTime;
	DWORD sleepStartTime;
	DWORD delayToAttackStartTime;

	int last_face_ani = RED_FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT;
	int last_attack_ani = RED_FIRE_PIRANHA_ANI_FACE_DOWN_LEFT;

	int piranhaType;
	Area playerArea;
	CMario* player;

	CFirePiranha(CMario* mario, int piranhaType);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

	Area GetCurrentPlayerArea();
	void CreateFireball();
};


#pragma once
#include "GameObject.h"
#include "Mario.h"

class CGreenPiranha : public CGameObject
{
public:
	DWORD attackStartTime;
	DWORD sleepStartTime;

	float playerLeft, playerTop, playerRight, playerBottom;

	CMario* player;

	CGreenPiranha(CMario* mario);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	bool CheckPlayerInSafeZone(float pl, float pt, float pr, float pb);
};


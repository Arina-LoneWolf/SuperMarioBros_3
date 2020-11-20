#pragma once
#include "GameObject.h"

class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	
public:
	DWORD deadTime;
	bool died;

	CGoomba();
	virtual void SetState(int state);
};
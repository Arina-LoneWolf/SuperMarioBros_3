#pragma once
#include "GameObject.h"

class CKoopa : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopa();
	virtual void SetState(int state);
	~CKoopa();
};
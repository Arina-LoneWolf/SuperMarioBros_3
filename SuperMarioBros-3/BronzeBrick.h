#pragma once
#include "GameObject.h"
#include "BrokenBrickPiece.h"
#include "Timer.h"

class CBronzeBrick : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listPiece;
	CTimer* transformationTime = new CTimer(TRANSFORMATION_TIME);
	bool vanish;
	int transformation;

	CBronzeBrick(int transformation);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};


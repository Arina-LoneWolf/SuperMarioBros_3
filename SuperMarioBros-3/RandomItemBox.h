#pragma once
#include "GameObject.h"
#include "Timer.h"

class CRandomItemBox : public CGameObject
{
public:
	ItemOfBox itemType;

	CTimer* itemTiming = new CTimer(ITEM_CONVERSION_TIME);

	CRandomItemBox();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};


#pragma once
#include "GameObject.h"

class CBrickContainsItem : public CGameObject
{
public:
	int brickType;
	int itemType;

	CBrickContainsItem(int brickType, int itemType);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};
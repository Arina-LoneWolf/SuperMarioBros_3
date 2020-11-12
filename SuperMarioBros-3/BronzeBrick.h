#pragma once
#include "GameObject.h"

class CBronzeBrick : public CGameObject
{
public:
	CBronzeBrick();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


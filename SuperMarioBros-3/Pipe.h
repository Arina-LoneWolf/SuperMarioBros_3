#pragma once
#include "GameObject.h"

class CPipe : public CGameObject
{
public:
	int pipeType;

	CPipe(int pipeType);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


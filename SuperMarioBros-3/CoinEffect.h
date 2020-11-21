#pragma once
#include "Effect.h"

class CCoinEffect : public CEffect
{
public:
	CCoinEffect(float brickX, float brickY);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	~CCoinEffect();
};


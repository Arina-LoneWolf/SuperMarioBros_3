#pragma once
#include "Effect.h"
#include "MoneyEffect.h"

class CCoinEffect : public CEffect
{
public:
	float minPosY;
	float maxPosY;
	D3DXVECTOR2 moneyStartingPos;

	bool isFalling;
	bool vanish;

	CMoneyEffect* effect;

	CCoinEffect(float brickX, float brickY);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	~CCoinEffect();
};


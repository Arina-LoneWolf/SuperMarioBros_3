#pragma once
#include "Effect.h"
#include "ScoreEffect.h"

class CCoinEffect : public CEffect
{
public:
	float minPosY;
	float maxPosY;
	D3DXVECTOR2 moneyStartingPos;

	bool isFalling = false;
	bool vanish = false;

	CScoreEffect* effect;

	CCoinEffect(float brickX, float brickY);
	void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	~CCoinEffect();
};


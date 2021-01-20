#pragma once
#include "Effect.h"

class CScoreEffect : public CEffect
{
public:
	CScoreEffect(D3DXVECTOR2 position, int moneyAni = MONEY_EFFECT_100);
	void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	~CScoreEffect();
};


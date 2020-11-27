#include "CoinEffect.h"

CCoinEffect::CCoinEffect(float brickX, float brickY)
{
	effectTime = COIN_EFFECT_TIME;
	x = brickX + COIN_EFFECT_X_ADDEND;
	y = brickY - COIN_EFFECT_Y_SUBTRAHEND;
	vy = -COIN_EFFECT_SPEED_Y;
	minPosY = brickY - COIN_EFFECT_MIN_Y_SUBTRAHEND;
	maxPosY = brickY - COIN_EFFECT_MAX_Y_SUBTRAHEND;
	moneyStartingPos = { brickX + MONEY_EFFECT_X_AT_BRICK_ADDEND, maxPosY - MONEY_EFFECT_Y_AT_BRICK_SUBTRAHEND };
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(16));
}

void CCoinEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CEffect::Update(dt, coObjects);

	y += dy;

	if (y < minPosY)
	{
		vy = -vy;
		isFalling = true;
	}

	if (isFalling && y > maxPosY)
	{
		vanish = true;
		isFalling = false;
		effect = new CMoneyEffect(moneyStartingPos);
	}
	
	if (effect)
		effect->Update(dt, coObjects);
}

void CCoinEffect::Render()
{
	if (!vanish)
		animation_set->at(0)->Render(x, y);

	if (effect)
		effect->Render();
}

CCoinEffect::~CCoinEffect()
{
}

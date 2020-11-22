#include "MoneyEffect.h"

CMoneyEffect::CMoneyEffect(D3DXVECTOR2 position, int moneyAni)
{
	x = position.x;
	y = position.y;
	ani = moneyAni;
	vy = -MONEY_EFFECT_SPEED_Y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
}

void CMoneyEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
}

void CMoneyEffect::Render()
{
	animation_set->at(ani)->Render(x, y);
}

CMoneyEffect::~CMoneyEffect()
{
}

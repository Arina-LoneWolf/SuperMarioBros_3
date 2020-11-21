#include "MoneyEffect.h"

CMoneyEffect::CMoneyEffect(D3DXVECTOR2 position, int moneyAni)
{
	effectTime = 100;
	x = position.x;
	y = position.y;
	ani = moneyAni;
	vy = -0.04f; //0.1f
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

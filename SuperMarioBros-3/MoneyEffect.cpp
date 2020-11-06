#include "MoneyEffect.h"

CMoneyEffect::CMoneyEffect(D3DXVECTOR2 position)
{
	x = position.x;
	y = position.y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(11));
}

void CMoneyEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

CMoneyEffect::~CMoneyEffect()
{
}

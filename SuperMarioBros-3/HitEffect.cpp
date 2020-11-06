#include "HitEffect.h"

CHitEffect::CHitEffect(D3DXVECTOR2 position)
{
	effectTime = HIT_EFFECT_TIME;
	x = position.x;
	y = position.y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(11));
}

void CHitEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

CHitEffect::~CHitEffect()
{
}

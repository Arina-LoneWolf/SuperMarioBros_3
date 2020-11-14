#include "HitEffect.h"

CHitEffect::CHitEffect(D3DXVECTOR2 position, int nx)
{
	effectTime = HIT_EFFECT_TIME;

	if (nx > 0)
	{
		x = position.x;
		y = position.y;
	}
	else
	{
		x = position.x - FIREBALL_BBOX_WIDTH;
		y = position.y;
	}
	
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(11));
}

void CHitEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

CHitEffect::~CHitEffect()
{
}

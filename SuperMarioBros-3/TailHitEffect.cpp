#include "TailHitEffect.h"

CTailHitEffect::CTailHitEffect(D3DXVECTOR2 position, int nx)
{
	effectTime = TAIL_HIT_EFFECT_TIME;

	if (nx > 0)
	{
		x = position.x;
		y = position.y - 6;
	}
	else
	{
		x = position.x - (TAIL_HIT_EFFECT_WIDTH - TAIL_BBOX_WIDTH);
		y = position.y - 6;
	}

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(13));
}

void CTailHitEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

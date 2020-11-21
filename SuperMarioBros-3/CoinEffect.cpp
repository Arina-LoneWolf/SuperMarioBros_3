#include "CoinEffect.h"

CCoinEffect::CCoinEffect(float brickX, float brickY)
{
	effectTime = 800;
	x = brickX - 4;
	y = brickY - 14;
	vy = 1.0f;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(6));
}

void CCoinEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt);

	y += vy;

	//if (y < )
}

void CCoinEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

CCoinEffect::~CCoinEffect()
{
}

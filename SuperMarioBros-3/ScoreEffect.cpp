#include "ScoreEffect.h"

CScoreEffect::CScoreEffect(D3DXVECTOR2 position, int moneyAni)
{
	effectTime = 800;
	x = position.x;
	y = position.y;
	ani = moneyAni;
	vy = -MONEY_EFFECT_SPEED_Y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(17));
}

void CScoreEffect::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CEffect::Update(dt, coObjects);
	
	y += dy;
}

void CScoreEffect::Render()
{
	animation_set->at(ani)->Render(x, y);
}

CScoreEffect::~CScoreEffect()
{
}

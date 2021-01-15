#include "Boomerang.h"
#include "Mario.h"

CBoomerang::CBoomerang(D3DXVECTOR2 position, int nx)
{
	type = Type::BOOMERANG;
	category = Category::WEAPON;
	// chỉnh lại vị trí
	if (nx > 0)
	{
		x = position.x + 10;
		y = position.y + 6;
	}
	else
	{
		x = position.x - 6;
		y = position.y + 6;
	}

	this->nx = nx;

	delayToHurl->Start();

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(27));
}

void CBoomerang::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (y <= BOOMERANG_FALLING_LIMIT_Y)
		vy += BOOMERANG_GRAVITY * dt;
	else
		vy = 0;

	x += dx;
	y += dy;
	
	float ml, mt, mr, mb, pl, pt, pr, pb; // main object - boomerang (m) and the player (p)
	GetBoundingBox(ml, mt, mr, mb);
	CMario::GetInstance()->GetBoundingBox(pl, pt, pr, pb);

	if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
	{
		if (!CMario::GetInstance()->GetUntouchable())
		{
			CreateEffect();
			CMario::GetInstance()->CollideWithEnemy();
		}
	}
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BOOMERANG_BBOX_WIDTH;
	b = t + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::Render()
{
	if (nx > 0)
		animation_set->at(BOOMERANG_TO_RIGHT)->Render(x, y);
	else
		animation_set->at(BOOMERANG_TO_LEFT)->Render(x, y);

	//RenderBoundingBox();
}

void CBoomerang::CreateEffect()
{
	CHitEffect* effect = new CHitEffect({ x, y }, nx);
	listEffect.push_back(effect);
}

CBoomerang::~CBoomerang()
{
}

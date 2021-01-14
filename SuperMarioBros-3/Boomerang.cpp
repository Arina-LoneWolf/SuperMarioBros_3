#include "Boomerang.h"

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
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(27));
}

void CBoomerang::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CBoomerang::Render()
{
	if (nx > 0)
		animation_set->at(BOOMERANG_TO_RIGHT)->Render(x, y);
	else
		animation_set->at(BOOMERANG_TO_LEFT)->Render(x, y);

	//RenderBoundingBox();
}

CBoomerang::~CBoomerang()
{
}

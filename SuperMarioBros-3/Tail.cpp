#include "Tail.h"

CTail::CTail(D3DXVECTOR2 position, int nx)
{
	type = TAIL;
	category = WEAPON;
	x = position.x;
	y = position.y;
	this->nx = nx;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CTail::Render()
{
	//RenderBoundingBox();
}

CTail::~CTail()
{
}

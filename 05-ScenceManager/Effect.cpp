#include "Effect.h"

CEffect::CEffect()
{
	category = EFFECT;
	startEffect = GetTickCount(); // i think this variable shouldn't be assigned here.
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (GetTickCount() - startEffect > effectTime)
		isFinishedUsing = true;
}

CEffect::~CEffect()
{
}

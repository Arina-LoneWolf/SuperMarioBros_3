#pragma once
#include "GameObject.h"

class CTail : public CGameObject
{
public:
	CTail(D3DXVECTOR2 position, int nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CTail();
};


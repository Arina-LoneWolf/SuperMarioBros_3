#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "HitEffect.h"

class CPiranhaFireball : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listEffect;

	CMario* player;

	CPiranhaFireball(D3DXVECTOR2 piranhaPos, Area playerArea, CMario* player);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();

	void SetRoute(Area playerArea);
	void SetNx(Area playerArea);
	void CreateEffect();
};


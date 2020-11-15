#pragma once
#include "GameObject.h"
#include "Mario.h"

class CPiranhaFireball : public CGameObject
{
public:
	DWORD collisionStartTime;
	bool collisionIsDone;

	CPiranhaFireball(D3DXVECTOR2 piranhaPos, Area playerArea);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();

	void SetRoute(Area playerArea);
	void SetNx(Area playerArea);
};


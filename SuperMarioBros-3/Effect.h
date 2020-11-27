#pragma once
#include "GameObject.h"

class CEffect : public CGameObject
{
protected:
	ULONGLONG startEffect;
	int effectTime;
public:
	CEffect();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* objects);
	virtual void Render() = 0;
	~CEffect();
};


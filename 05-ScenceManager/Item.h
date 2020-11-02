#pragma once
#include "GameObject.h"
#include "Mario.h"
#define SUPER_MUSHROOM	0
#define	SUPER_LEAF		1
#define FIRE_FLOWER		2

class CItem : public CGameObject
{
	bool spawn = true;
public:
	int change_direction = 1;
	float Start_y;
	float Start_x;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	CItem(float start_x, float start_y);
};


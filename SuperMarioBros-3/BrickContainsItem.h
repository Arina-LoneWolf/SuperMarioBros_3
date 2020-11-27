#pragma once
#include "GameObject.h"
#include "Mario.h"

class CBrickContainsItem : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listItem;
	CMario* player;

	int brickType;
	int itemType;

	float originalPosY;
	bool rammed;
	bool isAboutToDropItem;
	bool dropped;

	CBrickContainsItem(int brickType, int itemType, float originalPosY);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};
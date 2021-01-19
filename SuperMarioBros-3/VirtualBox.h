#pragma once
#include "GameObject.h"

class CVirtualBox : public CGameObject
{
public:
	bool collideWithBronzeBrick;
	bool collideWithBrick;

	CVirtualBox();
	bool CollideWithBrick(vector<LPGAMEOBJECT>* coObjects);
	bool CollidedHorizontally(vector<LPGAMEOBJECT>* coObjects, int koopaState);
	bool CollideWithGround(vector<LPGAMEOBJECT>* coObjects, int koopaState);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


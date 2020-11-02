#pragma once
#include "GameObject.h"

#define GROUNDTILE_BBOX_WIDTH 144 //152
#define GROUNDTILE_BBOX_HEIGHT 32

class CGroundTile : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


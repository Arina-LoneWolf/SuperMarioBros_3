#pragma once
#include "GameObject.h"

class CP_Switch : public CGameObject
{
public:
	bool readyToPerform = true;

	CP_Switch();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void PerformBricksTransformation(vector<LPGAMEOBJECT>* listBricks);
};


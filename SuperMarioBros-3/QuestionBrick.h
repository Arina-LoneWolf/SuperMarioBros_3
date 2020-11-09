#pragma once
#include "GameObject.h"

class CQuestionBrick : public CGameObject
{
public:
	CQuestionBrick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
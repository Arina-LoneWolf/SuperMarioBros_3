#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"
#include "Mario.h"

class CP_Meter
{
	LPANIMATION_SET powerMeter;

public:
	CMario* player;
	PowerMeter powerState;

	CP_Meter(CMario* player);
	void Update();
	void Render(float x, float y);
	~CP_Meter();
};


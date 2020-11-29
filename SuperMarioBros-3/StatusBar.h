#pragma once
#include "Animations.h"
#include "Sprites.h"
#include "Mario.h"
#include "Number.h"
#include "P_Meter.h"
#include "Timer.h"

class CStatusBar
{
	LPANIMATION_SET CRUD;
	CNumber number;
	CP_Meter* p_meter;
	CTimer* playTime = new CTimer(MAX_PLAY_TIME * CLOCKS_PER_SEC);
	ULONGLONG countdown;

public:
	CMario* player;

	CStatusBar(CMario* player);

	void Update(DWORD dt); // ghi tạm, suy nghĩ xem thêm cái gì vào nữa
	void Render(int worldID, float camX, float camY);

	~CStatusBar();
};


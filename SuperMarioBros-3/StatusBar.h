#pragma once
#include "Animations.h"
#include "Sprites.h"
#include "Mario.h"
#include "Number.h"
#include "P_Meter.h"
#include "Timer.h"

class CStatusBar
{
	LPANIMATION_SET item;
	LPSPRITE HUD;
	LPSPRITE itemBoxSet;
	LPSPRITE courseClear;
	LPSPRITE youGotACard;
	LPSPRITE blackBG;
	CNumber number;
	CP_Meter* p_meter;
	CTimer* playTime = new CTimer(MAX_PLAY_TIME * CLOCKS_PER_SEC);
	CTimer* flickerTime = new CTimer(FLICKER_TIME);
	CTimer* delayTimeToGotCard = new CTimer(GOT_CARD_DELAY_TIME);
	int countdown = 0;
	int item_ani;

	float cardPosX = YOU_GOT_A_CARD_POS_X + CARD_POS_X_ADDEND;
	float cardPosY = YOU_GOT_A_CARD_POS_Y + CARD_POS_Y_ADDEND;
public:
	CMario* player;

	CStatusBar(CMario* player);

	void Update();
	void Render(float camX, float camY);

	~CStatusBar();
};


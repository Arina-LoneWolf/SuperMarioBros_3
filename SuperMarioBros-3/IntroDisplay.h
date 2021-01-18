#pragma once
#include "Animations.h"
#include "Sprites.h"
#include "Timer.h"
#include "define.h"

class CIntroDisplay
{
	LPSPRITE curtain;
	LPANIMATION_SET gameLogo;
	LPANIMATION number_3;
	LPSPRITE cursor;

	CTimer* logoVibrateTime = new CTimer(LOGO_VIBRATE_TIME);
	CTimer* delayTimeToRevealCurtain = new CTimer(REVEAL_DELAY_TIME);

	float curtainPosY = 0;
	float gameLogoPosY = LOGO_INITIAL_POS_Y;
	bool logoIsVibrating;
	bool fullDisplay;
	bool cursorPos = 0;
	bool displaySelectionZone;
	int alpha = ALPHA_MAX_VALUE;
	int colorSubtrahend;

public:
	CIntroDisplay();

	void Update(ULONGLONG dt);
	void Render();
	void DisplaySelectionZone();
	void SetCursorPosition(bool cursorPos) { this->cursorPos = cursorPos; }
	bool GetCursorPosition() { return cursorPos; }

	~CIntroDisplay();
};


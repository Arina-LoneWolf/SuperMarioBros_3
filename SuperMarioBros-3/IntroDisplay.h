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

	CTimer* logoVibrateTime = new CTimer(600);
	CTimer* delayTimeToRevealCurtain = new CTimer(1000);

	float curtainPosY;
	float gameLogoPosY = -113;
	bool logoIsVibrating;
	bool fullDisplay;
	bool cursorPos = 0;
	bool displaySelectionZone;
	int alpha = 255;
	int colorSubtrahend;

public:
	//bool cursorPos = 0;
	
	CIntroDisplay();

	void Update(ULONGLONG dt);
	void Render();
	void DisplaySelectionZone();
	void SetCursorPosition(bool cursorPos) { this->cursorPos = cursorPos; }
	bool GetCursorPosition() { return cursorPos; }

	~CIntroDisplay();
};


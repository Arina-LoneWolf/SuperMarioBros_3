#include "IntroDisplay.h"
#include "Game.h"
#include "Textures.h"

CIntroDisplay::CIntroDisplay()
{
	curtain = CSprites::GetInstance()->Get(108);
	gameLogo = CAnimationSets::GetInstance()->Get(1);
	cursor = CSprites::GetInstance()->Get(107);
	number_3 = CAnimations::GetInstance()->Get(23);
	delayTimeToRevealCurtain->Start();
}

void CIntroDisplay::Update(ULONGLONG dt)
{
	if (delayTimeToRevealCurtain->IsTimeUp())
	{
		if (curtainPosY + CURTAIN_HEIGHT >= CGame::GetInstance()->GetCamPosY())
			curtainPosY += -CURTAIN_SPEED_Y * dt;
		else if (gameLogoPosY < LOGO_LIMIT_POS_Y)
			gameLogoPosY += LOGO_SPEED_Y * dt;
	}
	
	if (gameLogoPosY >= LOGO_LIMIT_POS_Y && logoVibrateTime->IsStopped())
	{
		gameLogoPosY = LOGO_LIMIT_POS_Y;
		logoIsVibrating = true;
		logoVibrateTime->Start();
	}

	if (logoVibrateTime->IsTimeUp())
	{
		logoIsVibrating = false;
		displaySelectionZone = true;
	}

	if (alpha == 0)
		fullDisplay = true;
}

void CIntroDisplay::Render()
{
	if (cursorPos == CURSOR_AT_1_PLAYER)
		cursor->Draw(CURSOR_POS_X, CURSOR_POS_Y_1_PLAYER);
	else
		cursor->Draw(CURSOR_POS_X, CURSOR_POS_Y_2_PLAYER);

	DisplaySelectionZone();

	if (logoIsVibrating)
		gameLogo->at(LOGO_ANI_VIBRATE)->Render(0, gameLogoPosY);
	else
		gameLogo->at(LOGO_ANI_NORMAL)->Render(0, gameLogoPosY);

	if (fullDisplay) // condition: the background is fully lit up 
		number_3->Render(NUMBER_3_POS_X, NUMBER_3_POS_Y);

	curtain->Draw(0, curtainPosY);
}

void CIntroDisplay::DisplaySelectionZone()
{
	if (displaySelectionZone)
	{
		colorSubtrahend += COLOR_ADDEND_LEVEL_UP;
		alpha = floor(alpha - colorSubtrahend);
		if (alpha < ALPHA_MIN_VALUE)
			alpha = ALPHA_MIN_VALUE;
	}

	CSprites::GetInstance()->Get(TRANSITION_SPRITE_ID)->Draw(0, 0, alpha);
}

CIntroDisplay::~CIntroDisplay()
{
}

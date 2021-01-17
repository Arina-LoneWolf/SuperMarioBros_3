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
			curtainPosY += -0.11f * dt;
		else if (gameLogoPosY < 22)
			gameLogoPosY += 0.3f * dt;
	}

	if (gameLogoPosY > 22)
		gameLogoPosY = 22;
	
	if (gameLogoPosY >= 22 && logoVibrateTime->IsStopped())
	{
		logoIsVibrating = true;
		logoVibrateTime->Start();
	}

	if (logoVibrateTime->IsTimeUp())
	{
		logoIsVibrating = false;
		displaySelectionZone = true;
	}
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

	if (fullDisplay) // dk: màn hình đã sáng lên 
		number_3->Render(NUMBER_3_POS_X, NUMBER_3_POS_Y);

	curtain->Draw(0, curtainPosY);
}

void CIntroDisplay::DisplaySelectionZone()
{
	LPDIRECT3DTEXTURE9 darken = CTextures::GetInstance()->Get(ID_TEX_DARKEN);
	RECT rect;

	float l = CGame::GetInstance()->GetCamPosX();
	float t = CGame::GetInstance()->GetCamPosY();

	rect.left = 0;
	rect.top = 0;
	rect.right = ceil(5 * SCREEN_WIDTH / 14);
	rect.bottom = 185;

	if (displaySelectionZone)
	{
		colorSubtrahend += COLOR_ADDEND_LEVEL_UP;
		alpha = floor(alpha - colorSubtrahend);
		if (alpha < 0)
		{
			alpha = 0;
			fullDisplay = true;
		}
	}

	CGame::GetInstance()->Draw(l, t, darken, rect.left, rect.top, rect.right, rect.bottom, alpha);
}

CIntroDisplay::~CIntroDisplay()
{
}

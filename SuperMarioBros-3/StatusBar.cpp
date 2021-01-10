#include "StatusBar.h"

CStatusBar::CStatusBar(CMario* player)
{
	this->player = player;
	playTime->Start();
	p_meter = new CP_Meter(player);
	HUD = CAnimationSets::GetInstance()->Get(21);
	blackBG = CSprites::GetInstance()->Get(11111);
}

void CStatusBar::Update()
{
	countdown = MAX_PLAY_TIME - (int)(playTime->GetElapsedTime() / CLOCKS_PER_SEC);
	p_meter->Update();
}

void CStatusBar::Render(float camX, float camY)
{
	float HUDPosX = camX + HUD_POS_X_ADDEND;
	float HUDPosY = camY + SCREEN_HEIGHT / SCREEN_DIVISOR - HUD_POS_Y_SUBTRAHEND;

	blackBG->Draw(camX, HUDPosY - 1);

	HUD->at(0)->Render(HUDPosX, HUDPosY);

	p_meter->Render(HUDPosX + P_METER_POS_X, HUDPosY + P_METER_POS_Y);

	number.Render(HUDPosX + WORLD_NUM_POS_X, HUDPosY + WORLD_NUM_POS_Y, WORLD_ID, WORLD_NUM_MAX_SIZE);
	number.Render(HUDPosX + LIVES_NUM_POS_X, HUDPosY + LIVES_NUM_POS_Y, player->GetLives(), LIVES_NUM_MAX_SIZE);
	number.Render(HUDPosX + SCORE_NUM_POS_X, HUDPosY + SCORE_NUM_POS_Y, player->GetScore(), SCORE_NUM_MAX_SIZE);
	number.Render(HUDPosX + MONEY_NUM_POS_X, HUDPosY + MONEY_NUM_POS_Y, player->GetMoney(), MONEY_NUM_MAX_SIZE);
	number.Render(HUDPosX + TIME_REMAINING_NUM_POS_X, HUDPosY + TIME_REMAINING_NUM_POS_Y, countdown, TIME_REMAINING_NUM_MAX_SIZE);
}

CStatusBar::~CStatusBar()
{
}

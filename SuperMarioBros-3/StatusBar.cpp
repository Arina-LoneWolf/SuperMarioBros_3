#include "StatusBar.h"

CStatusBar::CStatusBar(CMario* player)
{
	this->player = player;
	playTime->Start();
	p_meter = new CP_Meter(player);
	CRUD = CAnimationSets::GetInstance()->Get(21);
	blackBG = CSprites::GetInstance()->Get(11111);
}

void CStatusBar::Update()
{
	countdown = MAX_PLAY_TIME - (int)(playTime->GetElapsedTime() / CLOCKS_PER_SEC);
	p_meter->Update();
}

void CStatusBar::Render(float camX, float camY)
{
	float CRUDPosX = camX + CRUD_POS_X_ADDEND;
	float CRUDPosY = camY + SCREEN_HEIGHT / SCREEN_DIVISOR - CRUD_POS_Y_SUBTRAHEND;

	blackBG->Draw(camX, CRUDPosY - 1);

	CRUD->at(0)->Render(CRUDPosX, CRUDPosY);

	p_meter->Render(CRUDPosX + P_METER_POS_X, CRUDPosY + P_METER_POS_Y);

	number.Render(CRUDPosX + WORLD_NUM_POS_X, CRUDPosY + WORLD_NUM_POS_Y, WORLD_ID, WORLD_NUM_MAX_SIZE);
	number.Render(CRUDPosX + LIVES_NUM_POS_X, CRUDPosY + LIVES_NUM_POS_Y, player->GetLives(), LIVES_NUM_MAX_SIZE);
	number.Render(CRUDPosX + SCORE_NUM_POS_X, CRUDPosY + SCORE_NUM_POS_Y, player->GetScore(), SCORE_NUM_MAX_SIZE);
	number.Render(CRUDPosX + MONEY_NUM_POS_X, CRUDPosY + MONEY_NUM_POS_Y, player->GetMoney(), MONEY_NUM_MAX_SIZE);
	number.Render(CRUDPosX + TIME_REMAINING_NUM_POS_X, CRUDPosY + TIME_REMAINING_NUM_POS_Y, countdown, TIME_REMAINING_NUM_MAX_SIZE);
}

CStatusBar::~CStatusBar()
{
}

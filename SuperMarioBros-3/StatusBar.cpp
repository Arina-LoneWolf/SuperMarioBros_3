#include "StatusBar.h"

CStatusBar::CStatusBar(CMario* player)
{
	this->player = player;
	playTime->Start(); // đặt tạm ở đây
	p_meter = new CP_Meter(player);
	crud = CAnimationSets::GetInstance()->Get(21);
}

void CStatusBar::Update(DWORD dt)
{
	ULONGLONG now = GetTickCount64();
	countdown = MAX_PLAY_TIME - ((now - playTime->GetElapsedTime()) / CLOCKS_PER_SEC);
	p_meter->Update();
}

void CStatusBar::Render(int worldID, float camX, float camY)
{
	float crudX = camX + CRUD_POS_X_ADDEND;
	float crudY = camY + SCREEN_HEIGHT / 2 - CRUD_POS_Y_SUBTRAHEND;

	crud->at(0)->Render(crudX, crudY);

	p_meter->Render(crudX + P_METER_POS_X, crudY + P_METER_POS_Y);

	number.Render(crudX + WORLD_NUM_POS_X, crudY + WORLD_NUM_POS_Y, worldID, WORLD_NUM_MAX_SIZE);
	number.Render(crudX + LIVES_NUM_POS_X, crudY + LIVES_NUM_POS_Y, player->GetLives(), LIVES_NUM_MAX_SIZE);
	number.Render(crudX + SCORE_NUM_POS_X, crudY + SCORE_NUM_POS_Y, player->GetScore(), SCORE_NUM_MAX_SIZE);
	number.Render(crudX + MONEY_NUM_POS_X, crudY + MONEY_NUM_POS_Y, player->GetMoney(), MONEY_NUM_MAX_SIZE);
	number.Render(crudX + TIME_REMAINING_NUM_POS_X, crudY + TIME_REMAINING_NUM_POS_Y, countdown, TIME_REMAINING_NUM_MAX_SIZE);
}

CStatusBar::~CStatusBar()
{
}

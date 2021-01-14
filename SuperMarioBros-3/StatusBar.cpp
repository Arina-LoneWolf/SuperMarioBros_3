#include "StatusBar.h"

CStatusBar::CStatusBar(CMario* player)
{
	this->player = player;
	playTime->Start();
	p_meter = new CP_Meter(player);
	HUD = CSprites::GetInstance()->Get(1009);
	blackBG = CSprites::GetInstance()->Get(11111);
	itemBoxSet = CSprites::GetInstance()->Get(1038);
	courseClear = CSprites::GetInstance()->Get(1100);
	youGotACard = CSprites::GetInstance()->Get(1101);
	item = CAnimationSets::GetInstance()->Get(21);
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
	float itemBoxSetPosX = HUDPosX + ITEM_BOX_SET_POS_X_ADDEND;

	blackBG->Draw(camX, HUDPosY - BLACK_BG_POS_Y_SUBTRAHEND);

	HUD->Draw(HUDPosX, HUDPosY);

	itemBoxSet->Draw(itemBoxSetPosX, HUDPosY);

	p_meter->Render(HUDPosX + P_METER_POS_X, HUDPosY + P_METER_POS_Y);

	number.Render(HUDPosX + WORLD_NUM_POS_X, HUDPosY + WORLD_NUM_POS_Y, WORLD_ID, WORLD_NUM_MAX_SIZE);
	number.Render(HUDPosX + LIVES_NUM_POS_X, HUDPosY + LIVES_NUM_POS_Y, player->GetLives(), LIVES_NUM_MAX_SIZE);
	number.Render(HUDPosX + SCORE_NUM_POS_X, HUDPosY + SCORE_NUM_POS_Y, player->GetScore(), SCORE_NUM_MAX_SIZE);
	number.Render(HUDPosX + MONEY_NUM_POS_X, HUDPosY + MONEY_NUM_POS_Y, player->GetMoney(), MONEY_NUM_MAX_SIZE);
	number.Render(HUDPosX + TIME_REMAINING_NUM_POS_X, HUDPosY + TIME_REMAINING_NUM_POS_Y, countdown, TIME_REMAINING_NUM_MAX_SIZE);

	for (UINT i = 0; i < player->itemsPickedUp.size(); i++)
	{// xét điều kiện ở đây chưa đủ
		if (i == player->itemsPickedUp.size() - 1 && delayTimeToGotCard->IsTimeUp()) // is the item that Mario just picked up
		{
			switch (player->itemsPickedUp.at(i))
			{
			case ItemOfBox::STAR:
				item_ani = FLICKER_STAR_ANI;
				break;
			case ItemOfBox::MUSHROOM:
				item_ani = FLICKER_MUSHROOM_ANI;
				break;
			case ItemOfBox::FLOWER:
				item_ani = FLICKER_FLOWER_ANI;
				break;
			}
		}
		else if (i < player->itemsPickedUp.size() - 1)
		{
			//DebugOut(L"itempick size: %d", player->itemsPickedUp.size());
			switch (player->itemsPickedUp.at(i))
			{
			case ItemOfBox::STAR:
				item_ani = NORMAL_STAR_ANI;
				break;
			case ItemOfBox::MUSHROOM:
				item_ani = NORMAL_MUSHROOM_ANI;
				break;
			case ItemOfBox::FLOWER:
				item_ani = NORMAL_FLOWER_ANI;
				break;
			}
		}

		if (item_ani != -1)
			item->at(item_ani)->Render(itemBoxSetPosX + ITEM_POS_X_ADDEND + (ITEM_SPACE_X * i), HUDPosY + ITEM_POS_Y_ADDEND);
	}

	DebugOut(L"right edge of cam: %f\n", CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR);
	if (player->x > CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR)
	{
		DebugOut(L"VAO\n");
		courseClearPosX = camX + COURSE_CLEAR_POS_X_ADDEND;
		courseClearPosY = camY + COURSE_CLEAR_POS_Y_ADDEND;
		courseClear->Draw(courseClearPosX, courseClearPosY);
		if (delayTimeToGotCard->IsStopped())
			delayTimeToGotCard->Start();
	}

	if (delayTimeToGotCard->IsTimeUp())
	{
		float gotCardMessagePosX = courseClearPosX;
		float gotCardMessagePosY = courseClearPosY + YOU_GOT_A_CARD_POS_Y_ADDEND;
		float cardPosX = gotCardMessagePosX + CARD_POS_X_ADDEND;
		float cardPosY = gotCardMessagePosY + CARD_POS_Y_ADDEND;

		youGotACard->Draw(gotCardMessagePosX, gotCardMessagePosY);
		switch (player->itemsPickedUp.at(player->itemsPickedUp.size() - 1))
		{
		case ItemOfBox::STAR:
			item_card_ani = NORMAL_STAR_ANI;
			break;
		case ItemOfBox::MUSHROOM:
			item_card_ani = NORMAL_MUSHROOM_ANI;
			break;
		case ItemOfBox::FLOWER:
			item_card_ani = NORMAL_FLOWER_ANI;
			break;
		}
		item->at(item_card_ani)->Render(cardPosX, cardPosY);
	}
}

CStatusBar::~CStatusBar()
{
}

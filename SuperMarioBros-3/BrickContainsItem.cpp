#include "BrickContainsItem.h"

CBrickContainsItem::CBrickContainsItem(int brickType, int itemType, float originalPosY)
{
	category = MISC;
	type = BRICK_CONTAINS_ITEM;
	this->brickType = brickType;
	this->itemType = itemType;
	this->originalPosY = originalPosY;
}

void CBrickContainsItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	y += dy;

	if (y < originalPosY - BRICK_BOUNCE_UP_TO)
	{
		vy = -vy;
		rammed = true;
	}

	if (rammed && y >= originalPosY)
	{
		y = originalPosY;
		vy = 0;
	}

}

void CBrickContainsItem::Render()
{
	if (state == STATE_RAMMED) // chỗ này có thể đổi lại thành biến rammed (bool)
		ani = NORMAL_BRICK_ANI;
	else
	{
		if (brickType == TypeOfBrickContainsItem::QUESTION_BRICK)
			ani = QUESTION_BRICK_ANI;
		else if (brickType == TypeOfBrickContainsItem::DISGUISED_BRONZE_BRICK)
			ani = DISGUISED_BRONZE_BRICK_ANI;
	}
	
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBrickContainsItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIDE_LENGTH;
	b = y + BRICK_BBOX_SIDE_LENGTH;
}

void CBrickContainsItem::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_RAMMED)
	{
		// brick bounces and become normal, item appears
		if (!rammed)
			vy = -BRICK_DEFLECT_SPEED_Y;
		
		//DropItem();
	}
}

void CBrickContainsItem::DropItem()
{
	switch (itemType)
	{
	case ITEM_RANDOM:
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
		{
			// create super mushroom
		}
		else if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_FIRE)
		{
			// create super leaf
		}
		else if (player->GetLevel() == MARIO_RACCOON)
		{
			// create ice flower
		}
		break;
	case ITEM_MONEY:
		// create money effect
		break;
	case ITEM_UP_MUSHROOM:
		// create up mushroom
		break;
	case ITEM_P_SWITCH:
		// create P switch
		break;
	}
}

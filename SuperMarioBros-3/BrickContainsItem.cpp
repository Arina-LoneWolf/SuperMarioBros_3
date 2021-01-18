#include "BrickContainsItem.h"
#include "CoinEffect.h"

CBrickContainsItem::CBrickContainsItem(int brickType, int itemType, float originalPosY)
{
	category = Category::MISC;
	this->brickType = brickType;
	this->itemType = itemType;
	this->originalPosY = originalPosY;
	if (itemType == ItemOfBrick::ITEM_COIN_X10)
		coin = MAX_COIN_OF_BRICK;
}

void CBrickContainsItem::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
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
		isAboutToDropItem = true;

		if (coin > 0)
			rammed = false;
	}

}

void CBrickContainsItem::Render()
{
	if (state == STATE_RAMMED && coin == 0) // chỗ này có thể đổi lại thành biến rammed (bool)
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
		if (!rammed)
			vy = -BRICK_DEFLECT_SPEED_Y;
		if (itemType == ItemOfBrick::ITEM_COIN_X10 && coin > 0)
			coin--;
	}
}

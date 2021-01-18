#include "RandomItemBox.h"
#include "Mario.h"

CRandomItemBox::CRandomItemBox()
{
	itemType = ItemOfBox::STAR;
	category = Category::ITEM;
	itemTiming->Start();
}

void CRandomItemBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == STATE_EMPTY)
		return;

	left = x + ITEM_POS_X_OFFSET;
	top = y;
	right = left + ITEM_BBOX_WIDTH;
	bottom = y + ITEM_BBOX_HEIGHT;
}

ItemOfBox operator++(ItemOfBox& item)
{
	item = static_cast<ItemOfBox>(((int)item + 1) % (int)ItemOfBox::MAX_ITEM_TYPE_NUM);
	return item;
}

//ItemOfBox operator++(ItemOfBox& item, int)
//{
//	ItemOfBox result = item;
//	++item;
//	return result;
//}

void CRandomItemBox::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;

	if (itemTiming->IsTimeUp())
	{
		++itemType;
		itemTiming->Start();
	}
}

void CRandomItemBox::Render()
{
	//animation_set->at((int)itemType)->Render(x, y);
	if (state == STATE_EMPTY)
	{
		switch (itemType)
		{
		case ItemOfBox::STAR:
			ani = ITEM_BOX_EMPTY_STAR_ANI;
			break;
		case ItemOfBox::MUSHROOM:
			ani = ITEM_BOX_EMPTY_MUSHROOM_ANI;
			break;
		case ItemOfBox::FLOWER:
			ani = ITEM_BOX_EMPTY_FLOWER_ANI;
			break;
		}
	}
	else
	{
		switch (itemType)
		{
		case ItemOfBox::STAR:
			ani = ITEM_BOX_STAR_ANI;
			break;
		case ItemOfBox::MUSHROOM:
			ani = ITEM_BOX_MUSHROOM_ANI;
			break;
		case ItemOfBox::FLOWER:
			ani = ITEM_BOX_FLOWER_ANI;
			break;
		}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CRandomItemBox::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_EMPTY)
	{
		itemTiming->Stop();
		vy = -EMPTY_ITEM_SPEED_Y;
		CMario::GetInstance()->autoGoRight = true;
		CMario::GetInstance()->itemsPickedUp.push_back(itemType);
		CMario::GetInstance()->justPickedReward = true;
		CMario::GetInstance()->currentPoint->isConquered = true;
	}
}

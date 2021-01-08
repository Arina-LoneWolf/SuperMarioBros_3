#include "RandomItemBox.h"

CRandomItemBox::CRandomItemBox()
{
	itemType = ItemOfBox::STAR;
	category = Category::ITEM;
	itemTiming->Start();
}

void CRandomItemBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BBOX_WIDTH;
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
	if (itemTiming->IsTimeUp())
	{
		++itemType;
		itemTiming->Start();
	}
}

void CRandomItemBox::Render()
{
	animation_set->at((int)itemType)->Render(x, y);
}

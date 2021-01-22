#include "P_Switch.h"

CP_Switch::CP_Switch()
{
	x = P_SWITCH_POS_X;
	y = P_SWITCH_POS_Y;
	type = Type::P_SWITCH;
	category = Category::ITEM;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(18));
}

void CP_Switch::Render()
{
	if (state == STATE_PRESSED)
		ani = P_SWITCH_PRESSED_ANI;
	else
		ani = P_SWITCH_NORMAL_ANI;
	
	animation_set->at(ani)->Render(x, y);
}

void CP_Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	/*if (state == STATE_PRESSED)
		return;*/

	l = x;
	t = y;
	r = l + P_SWITCH_BBBOX_SIDE_LENGTH;
	b = t + P_SWITCH_BBBOX_SIDE_LENGTH;
}

void CP_Switch::PerformBricksTransformation(vector<LPGAMEOBJECT>* listBricks)
{
	for (UINT i = 0; i < listBricks->size(); i++)
	{
		listBricks->at(i)->SetState(STATE_TRANSFORMATION);
	}
}

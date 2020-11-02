#include "Item.h"
#include "Brick.h"
#include "Utils.h"
#define GRAVITY 0.3f
#define Item_move 62 // quang duong item di chuyen khi troi len

CItem::CItem(float start_x, float start_y)
{

	this->Start_x = start_x;
	this->Start_y = start_y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (!eaten)
	{
		left = x;
		top = y;
		right = x + 18;
		bottom = y + 18;
	}
}
void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (spawn) {
		//DebugOut(L"vy %f \n", vy);
		if (y > Item_move)
			vy = -0.05;
		else spawn = false;

	}
	else
	{
		vx = 0.05 * change_direction;
		vy = GRAVITY;

	}
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;


		if (coEvents.size() <= 3)
		{
			x += dx;
		}
		else
			change_direction *= -1;
		if (nx != 0) vx = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CItem::Render()
{
	if (!eaten)
		animation_set->at(state)->Render(x, y);
	//RenderBoundingBox();
}

void CItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SUPER_MUSHROOM:
		break;
	case SUPER_LEAF:
		break;
	}
}
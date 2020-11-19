#include "SuperLeaf.h"

CSuperLeaf::CSuperLeaf(float brickX, float brickY)
{
	type = SUPER_LEAF;
	category = ITEM;

	this->x = brickX;
	this->y = brickY - 7;
	vy = -SUPER_LEAF_DEFLECT_SPEED_Y;

	minPosY = brickY - 32;
	leftLimit = brickX;
	rightLimit = brickX + 32;

	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(5));
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// xử lý ra khỏi camera xóa
	DebugOut(L"vy = %f\n", vy);
	x += vx;
	y += vy;

	if (y <= minPosY)
	{
		isFalling = true;
		/*vy = SUPER_LEAF_SPEED_Y;
		vx = 3 * pow(10, vy);*/
	}

	if (isFalling)
	{
		if (x <= leftLimit)
		{
			vy = SUPER_LEAF_SPEED_Y;
			vx = 0.3 * pow(35, vy); //0.05  0.3 35
		}

		if (x >= rightLimit)
		{
			vy = SUPER_LEAF_SPEED_Y;
			vx = -0.3 * pow(35, vy); //0.1
		}
	}
}

void CSuperLeaf::Render()
{
	if (vx > 0)
		ani = SUPER_LEAF_ANI_RIGHT;
	else
		ani = SUPER_LEAF_ANI_LEFT;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CSuperLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SUPER_LEAF_BBOX_WIDTH;
	b = y + SUPER_LEAF_BBOX_HEIGHT;
}

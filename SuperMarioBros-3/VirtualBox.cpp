#include "VirtualBox.h"
#include "Mario.h"

CVirtualBox::CVirtualBox()
{
}

bool CVirtualBox::CollideWithBrick(vector<LPGAMEOBJECT>* coObjects)
{
	float vl, vt, vr, vb, bl, bt, br, bb; // virtual box - brick
	GetBoundingBox(vl, vt, vr, vb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->type != Type::BRONZE_BRICK && e->type != Type::BRICK_CONTAINS_ITEM && e->type != Type::COLOR_BOX)
			continue;
		e->GetBoundingBox(bl, bt, br, bb);
		if (CGameObject::CheckAABB(vl, vt, vr, vb, bl, bt, br, bb))
		{
			return true;
		}
	}
	return false;
}

bool CVirtualBox::CollidedHorizontally(vector<LPGAMEOBJECT>* coObjects, int koopaState)
{
	float vl, vt, vr, vb, ol, ot, or, ob; // virtual box - brick
	GetBoundingBox(vl, vt, vr, vb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->type == Type::GREEN_KOOPA || e->type == Type::GREEN_PARAKOOPA || e->type == Type::RED_KOOPA || e->type == Type::RED_PARAKOOPA || e->type == Type::COLOR_BOX)
			continue;
		e->GetBoundingBox(ol, ot, or, ob);
		if (CGameObject::CheckAABB(vl, vt, vr, vb, ol, ot, or, ob))
		{
			if (koopaState == KOOPA_STATE_SPIN_AND_MOVE)
			{
				if (e->type == Type::BRONZE_BRICK)
				{
					e->SetState(STATE_DESTROYED);
					CMario::GetInstance()->score += 10;
				}
				else if (e->type == Type::BRICK_CONTAINS_ITEM)
				{
					e->SetState(STATE_RAMMED);
				}
			}
			return true;
		}
	}
	return false;
}

bool CVirtualBox::CollideWithGround(vector<LPGAMEOBJECT>* coObjects, int koopaState)
{
	float vl, vt, vr, vb, ol, ot, or , ob; // virtual box - brick
	GetBoundingBox(vl, vt, vr, vb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->category != Category::MISC || e->type == Type::BRONZE_BRICK || e->type == Type::BRICK_CONTAINS_ITEM || e->type == Type::COLOR_BOX)
			continue;
		e->GetBoundingBox(ol, ot, or , ob);
		if (CGameObject::CheckAABB(vl, vt, vr, vb, ol, ot, or , ob))
		{
			if (koopaState == ENEMY_STATE_MOVE)
			{
				if (this->x + 4 <= e->x || this->x + 1 >= e->x + e->width)
					return true;
			}
		}
	}
	return false;
}

void CVirtualBox::Render()
{
	DebugOut(L"gagaag %f \n", y);
	//RenderBoundingBox();
}

void CVirtualBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y + 1;
	r = l + 7;
	b = t + 10;
}

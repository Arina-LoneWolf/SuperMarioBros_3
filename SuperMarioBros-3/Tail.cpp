#include "Tail.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float l, t, r, b, ln, tn, rn, bn;
	GetBoundingBox(l, t, r, b);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ln, tn, rn, bn);
		if (CGameObject::CheckAABB(l, t, r, b, ln, tn, rn, bn))
		{
			if (e->category == Category::ENEMY)
			{
				e->object_colliding_nx = this->nx;
				e->SetState(ENEMY_STATE_ATTACKED_BY_TAIL);
			}
		}
	}
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!appear)
		return;
	left = x;
	top = y;
	right = x + TAIL_BBOX_WIDTH;
	bottom = y + TAIL_BBOX_HEIGHT;
}

void CTail::Render()
{
	//RenderBoundingBox();
}

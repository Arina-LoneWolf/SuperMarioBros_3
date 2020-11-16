#include "Tail.h"

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (LPGAMEOBJECT effect : listEffect)
	{
		if (effect->isFinishedUsing)
			listEffect.erase(listEffect.begin());
	}

	for (LPGAMEOBJECT effect : listEffect)
		effect->Update(dt, coObjects);

	float ml, mt, mr, mb, ol, ot, or, ob; // main object (m) and the other one (o)
	GetBoundingBox(ml, mt, mr, mb); 
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ol, ot, or , ob);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, ol, ot, or, ob))
		{
			if (e->category == Category::ENEMY)
			{
				CreateEffect();
				e->object_colliding_nx = this->nx;
				e->SetState(ENEMY_STATE_ATTACKED_BY_TAIL);
			}
			else if (e->type == Type::BRONZE_BRICK)
			{
				e->SetState(STATE_DESTROYED);
			}
			else if (e->type == Type::BRICK_CONTAINS_ITEM)
			{
				e->SetState(STATE_RAMMED);
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
	for (LPGAMEOBJECT effect : listEffect)
		effect->Render();

	//RenderBoundingBox();
}

void CTail::CreateEffect()
{
	CTailHitEffect* effect = new CTailHitEffect({ x, y }, nx);
	listEffect.push_back(effect);
}

#include "BronzeBrick.h"

CBronzeBrick::CBronzeBrick(int transformation)
{
	if (transformation == BrickTransformation::BRICK_FORM)
	{
		type = BRONZE_BRICK;
		category = MISC;
	}
	else if (transformation == BrickTransformation::COIN_FORM)
	{
		type = COIN;
		category = ITEM;
	}
	this->transformation = transformation;
}

void CBronzeBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"vao update bronze brick\n");
	int num_vanishPiece = 0;

	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Update(dt, coObjects);
		if (piece->isFinishedUsing)
			num_vanishPiece++;
	}

	if (num_vanishPiece == 4)
		isFinishedUsing = true;

	if (transformationTime && GetTickCount64() - transformationTime > TRANSFORMATION_TIME)
		SetState(STATE_NORMAL);
}

void CBronzeBrick::Render()
{
	if (type == Type::BRONZE_BRICK)
		ani = BRONZE_BRICK_ANI;
	else if (type == Type::COIN)
	{
		if (y > HORIZONTAL_SEPARATION_LINE)
			ani = IDLE_COIN_ANI;
		else
			ani = ROTATING_COIN;
	}

	if (!vanish)
		animation_set->at(ani)->Render(x, y);

	for (LPGAMEOBJECT piece : listPiece)
		piece->Render();
}

void CBronzeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (vanish)
		return;
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIDE_LENGTH;
	b = y + BRICK_BBOX_SIDE_LENGTH;
}

void CBronzeBrick::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == STATE_DESTROYED)
	{
		CBrokenBrickPiece* topLeftPiece = new CBrokenBrickPiece({ x - 1, y - 2 }, -1, 2);
		CBrokenBrickPiece* topRightPiece = new CBrokenBrickPiece({ x + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_WIDTH + 1, y - 2 }, 1, 2);
		CBrokenBrickPiece* bottomLeftPiece = new CBrokenBrickPiece({ x - 1, y + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_HEIGHT }, -1);
		CBrokenBrickPiece* bottomRightPiece = new CBrokenBrickPiece({ x + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_WIDTH + 1, y + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_HEIGHT }, 1);

		listPiece = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
		vanish = true;
	}
	else if (state == STATE_TRANSFORMATION)
	{
		if (transformation == BrickTransformation::BRICK_FORM)
		{
			type = COIN;
			category = ITEM;
		}
		else if (transformation == BrickTransformation::COIN_FORM)
		{
			type = BRONZE_BRICK;
			category = MISC;
		}
		transformationTime = GetTickCount64();
	}
	else if (state == STATE_NORMAL)
	{
		if (transformation == BrickTransformation::BRICK_FORM)
		{
			type = BRONZE_BRICK;
			category = MISC;
		}
		else if (transformation == BrickTransformation::COIN_FORM)
		{
			type = COIN;
			category = ITEM;
		}
	}
}

#include "BronzeBrick.h"

CBronzeBrick::CBronzeBrick()
{
	type = BRONZE_BRICK;
	category = MISC;
}

void CBronzeBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int num_vanishPiece = 0;

	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Update(dt, coObjects);
		if (piece->isFinishedUsing)
			num_vanishPiece++;
	}

	if (num_vanishPiece == 4)
		isFinishedUsing = true;
}

void CBronzeBrick::Render()
{
	if (!vanish)
		animation_set->at(0)->Render(x, y);

	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Render();
	}
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
		CBrokenBrickPiece* topLeftPiece = new CBrokenBrickPiece({ x - 1, y - 2 }, -1);
		CBrokenBrickPiece* topRightPiece = new CBrokenBrickPiece({ x + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_WIDTH + 1, y - 2 }, 1);
		CBrokenBrickPiece* bottomLeftPiece = new CBrokenBrickPiece({ x - 1, y + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_HEIGHT }, -1);
		CBrokenBrickPiece* bottomRightPiece = new CBrokenBrickPiece({ x + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_WIDTH + 1, y + BRICK_BBOX_SIDE_LENGTH - BROKEN_BRICK_PIECE_HEIGHT }, 1);

		listPiece = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
		vanish = true;
	}
}

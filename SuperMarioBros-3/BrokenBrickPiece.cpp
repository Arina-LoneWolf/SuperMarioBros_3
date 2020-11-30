#include "BrokenBrickPiece.h"

CBrokenBrickPiece::CBrokenBrickPiece(D3DXVECTOR2 position, int nx, int deflectFactorY)
{
	x = position.x;
	y = position.y;
	vx = BROKEN_BRICK_PIECE_DEFLECT_SPEED_X * nx;
	vy = -BROKEN_BRICK_PIECE_DEFLECT_SPEED_Y * deflectFactorY;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(12));
}

void CBrokenBrickPiece::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += BROKEN_BRICK_PIECE_GRAVITY * dt;

	x += dx;
	y += dy;

	if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2)
		isFinishedUsing = true;
}

void CBrokenBrickPiece::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBrokenBrickPiece::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

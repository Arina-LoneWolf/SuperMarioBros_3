#pragma once
#include "GameObject.h"
#include "BrokenBrickPiece.h"

class CBronzeBrick : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listPiece;
	bool vanish;

	/*CBrokenBrickPiece* topLeftPiece;
	CBrokenBrickPiece* topRightPiece;
	CBrokenBrickPiece* bottomLeftPiece;
	CBrokenBrickPiece* bottomRightPiece;*/

	CBronzeBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};


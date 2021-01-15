#pragma once
#include "GameObject.h"
#include "Boomerang.h"
//#include "Timer.h"

class CBoomerangBrother : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listBoomerang;
	CTimer* delayTimeToRest = new CTimer(REST_TIME);
	CTimer* coolDown = new CTimer(2500);
	CTimer* delayToHurlSecBoomerang = new CTimer(1500);
	int randTurnToJump;
	int turnCounter;
	bool canJump;
	bool playerPosition = 1;

	CBoomerangBrother();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void CreateBoomerang();
	void GenerateRandomTurnToJump();
	bool GetPlayerDirection();
	~CBoomerangBrother();
};


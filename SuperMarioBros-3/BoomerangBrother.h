#pragma once
#include "GameObject.h"
#include "Boomerang.h"
#include "ScoreEffect.h"
//#include "Timer.h"

class CBoomerangBrother : public CGameObject
{
	static CBoomerangBrother* __instance;

public:
	vector<LPGAMEOBJECT> listBoomerang;
	CTimer* delayTimeToRest = new CTimer(BOOMERANG_BROTHER_REST_TIME);
	CTimer* coolDown = new CTimer(BOOMERANG_COOLDOWN_TIME);
	CTimer* delayToHurlSecBoomerang = new CTimer(HURLING_SEC_BOOMERANG_DELAY_TIME);
	CScoreEffect* effect;
	int randTurnToJump;
	int turnCounter;
	int playerPosition = 1;
	bool isHoldingBoomerang;
	bool died;

	CBoomerangBrother();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void CreateBoomerang();
	void GenerateRandomTurnToJump();
	int GetPlayerDirection();

	static CBoomerangBrother* GetInstance();

	~CBoomerangBrother();
};


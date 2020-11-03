#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Game.h"
#include "Floor.h"
#include "ColorBox.h"
#include "Fireball.h"
#include "HitEffect.h"



class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	vector<LPGAMEOBJECT> listWeapon;
	vector<LPGAMEOBJECT> listEffect;

	DWORD jumpStartTime;
	DWORD attackStartTime;
	DWORD waggingTailStartTime;
	bool isSpeedingUp, isFalling, isSitting, isWaggingTail, isFlying, isAttacking;
	bool canFlyUpFromGround;
	bool isOnGround;

	bool isWaitingForAni;
	bool specialAniCase;
	bool turnOn;
	int last_nx;
	int last_ani = -1;

	float last_y = INITIAL_LAST_Y;
	float last_vx;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	virtual void CollideWithItem(vector<LPGAMEOBJECT>* itemsList);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	int GetAni() { return ani; }
	//bool IsSpecialAniCase();

	CFireball* CreateFireball(float x, float y, int nx);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void TurnIntoSuper();
	void TurnIntoRaccoon();
	void TurnIntoFire();

	void DecelerateSharply();
	void DecelerateSlightly();

	void WaitForAni();

	//void SetAniDie();
	//void SetAniSit();
	//void SetAniWalkRight();
	//void SetAniWalkLeft();
	//void SetAniJump();
	//void SetAniRun();
	//void SetAniFly();
	//void SetAniIdle();
	
	void SetStateIdle();
	void SetStateWalkRight();
	void SetStateWalkLeft();

	void JumpHigh();
	void JumpLow();
	void ToRight();
	void ToLeft();
	void Idle();
	void Sit();
	void Fly();
	void Attack();
	void Stop();
};
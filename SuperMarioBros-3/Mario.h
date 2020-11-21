#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Floor.h"
#include "ColorBox.h"
#include "Fireball.h"
#include "Tail.h"
#include "HitEffect.h"
#include "Portal.h"


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

	CTail* tail = new CTail();

	DWORD jumpStartTime;
	DWORD attackStartTime;
	DWORD waggingTailStartTime;
	DWORD flyStartTime;
	DWORD kickStartTime;

	bool isFalling, isSitting, isWaggingTail, isFlying, isAttacking, isRunning;
	bool kickShell;
	bool isHoldingShell;
	bool canFly;
	bool isOnGround = true;
	bool immovable;
	bool isWaitingForAni;

	float last_y = INITIAL_LAST_Y;
	float last_vx;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	int GetUntouchable() { return untouchable; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void Reset();
	int GetAni() { return ani; }

	void CreateFireball();
	void CollideWithEnemy();
	void CheckCollisionWithItems(vector<LPGAMEOBJECT>* listItem);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void TurnIntoSuper();
	void TurnIntoRaccoon();
	void TurnIntoFire();

	void DecelerateSharply();
	void DecelerateSlightly();

	void Idle();
	void Sit();
	void Fly();
	void Attack();
};
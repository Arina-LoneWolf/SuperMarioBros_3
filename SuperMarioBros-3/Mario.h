#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Floor.h"
#include "ColorBox.h"
#include "Fireball.h"
#include "Tail.h"
#include "HitEffect.h"
#include "Portal.h"
#include "MapPoint.h"


class CMario : public CGameObject
{
	static CMario* __instance;

	int level;
	int untouchable;
	ULONGLONG untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	vector<LPGAMEOBJECT> listWeapons;
	vector<LPGAMEOBJECT> listEffects;

	CTail* tail = new CTail();

	ULONGLONG attackStartTime;
	ULONGLONG waggingTailStartTime;

	CTimer* jumpTime = new CTimer(MARIO_LOW_JUMP_TIME);
	CTimer* flyTime = new CTimer(MARIO_FLYING_TIME);
	CTimer* kickTime = new CTimer(MARIO_KICK_TIME);
	//CTimer* stopTime = new CTimer(200);

	int score = 0;
	int money = 0;
	int lives = INITIAL_PLAYER_LIVES;
	ItemOfBox hud1 = ItemOfBox::MUSHROOM;
	ItemOfBox hud2 = ItemOfBox::STAR;
	vector<ItemOfBox> itemsPickedUp/*{ hud1, hud2 }*/;

	CMapPoint* currentPoint = new CMapPoint(START_POINT_X, START_POINT_Y, 4, 0, 1, 0, 0);
	CMapPoint* nextPoint = new CMapPoint(64, 48, 0, 1, 0, 1, 0);

	int GetScore() { return score; }
	int GetMoney() { return money; }
	int GetLives() { return lives; }

	bool isFalling, isSitting, isWaggingTail, isFlying, isAttacking, isRunning, isStopping;
	bool kickShell;
	bool isHoldingShell;
	bool canFly; // only for Raccoon
	bool isOnGround = true;
	bool immovable;
	bool unpressDown;
	bool onPressUp;
	bool renderBBOX;
	bool autoGoRight;
	bool inStartOfPipe;
	bool inEndOfPipe;
	bool readyToOutOfPipe;
	bool goHiddenArea;
	bool leaveHiddenArea;
	bool screenDim;
	bool onOverworldMap = true;
	bool justPickedReward;
	bool isOnFloatingWood;
	bool walkByAutoCam;
	bool pauseCam;
	bool goBackyard;
	bool onBackyardPipe;

	vector<bool> movementPermission{ 0, 1, 0, 0 };

	float last_y = INITIAL_LAST_Y;
	float last_vx;
	float last_vy;
	float y_when_started_to_jump;

	CMario(float x = 0.0f, float y = 0.0f);
	void UpdateAtOverworldMap(ULONGLONG dt, vector<LPGAMEOBJECT>* coPoints = NULL);
	void RenderAtOverworldMap();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	static CMario* GetInstance();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	int GetUntouchable() { return untouchable; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void Reset();
	void RefreshAtPlayScene();
	void RefreshAtOverworldMap();
	int GetAni() { return ani; }

	void CreateFireball();
	void CollideWithEnemy();
	void CheckCollisionWithItems(vector<LPGAMEOBJECT>* listItem);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	float GetLeft();
	float GetTop();
	float GetBottom();

	void SetPositionAtCurrentPoint(float x, float y);

	void DecelerateSharply();
	void DecelerateSlightly();

	void Idle();
	void Sit();
	void Fly();
	void Attack();
};
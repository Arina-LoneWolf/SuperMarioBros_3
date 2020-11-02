#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Game.h"
#include "Floor.h"
#include "ColorBox.h"

#define MARIO_RUNNING_SPEED						0.3f
#define MARIO_MAX_WALKING_SPEED					0.15f
#define MARIO_RUNNING_ACCELERATION				0.0002f
#define MARIO_WALKING_ACCELERATION				0.0002f
#define MARIO_SMALL_ACCELERATION_SUBTRAHEND		0.0003f
#define MARIO_LARGE_ACCELERATION_SUBTRAHEND		0.0005f
#define INITIAL_LAST_Y							134.0f

#define MARIO_HIGH_JUMP_SPEED_Y		0.275f
#define MARIO_LOW_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_GRAVITY				0.0006f
#define MARIO_LOW_JUMP_GRAVITY		0.001f
#define MARIO_DIE_DEFLECT_SPEED		0.5f
#define MARIO_FLY_SPEED_Y			0.25f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP_LOW		300
#define MARIO_STATE_JUMP_HIGH		400
#define	MARIO_STATE_SIT_DOWN		500
#define MARIO_STATE_RUNNING_RIGHT	600
#define MARIO_STATE_RUNNING_LEFT	700
#define MARIO_STATE_FLYING			800
#define MARIO_STATE_STOP			900
#define MARIO_STATE_ATTACK			1000

#define MARIO_STATE_DIE				999

#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_WALKING_RIGHT			1
#define MARIO_ANI_BIG_RUNNING_RIGHT			2
#define MARIO_ANI_BIG_JUMP_RIGHT			3
#define MARIO_ANI_BIG_SITTING_RIGHT			4
#define MARIO_ANI_BIG_STOP_RIGHT			5
#define MARIO_ANI_BIG_FALLING_RIGHT			6

#define MARIO_ANI_BIG_IDLE_LEFT				7
#define MARIO_ANI_BIG_WALKING_LEFT			8
#define MARIO_ANI_BIG_RUNNING_LEFT			9
#define MARIO_ANI_BIG_JUMP_LEFT				10
#define MARIO_ANI_BIG_SITTING_LEFT			11
#define MARIO_ANI_BIG_STOP_LEFT				12
#define MARIO_ANI_BIG_FALLING_LEFT			13

#define MARIO_ANI_SMALL_IDLE_RIGHT			14
#define MARIO_ANI_SMALL_WALKING_RIGHT		15
#define MARIO_ANI_SMALL_RUNNING_RIGHT		16
#define MARIO_ANI_SMALL_JUMP_RIGHT			17
#define MARIO_ANI_SMALL_STOP_RIGHT			18

#define MARIO_ANI_SMALL_IDLE_LEFT			19
#define MARIO_ANI_SMALL_WALKING_LEFT		20
#define MARIO_ANI_SMALL_RUNNING_LEFT		21
#define MARIO_ANI_SMALL_JUMP_LEFT			22
#define MARIO_ANI_SMALL_STOP_LEFT			23

#define MARIO_RACCOON_ANI_IDLE_RIGHT				24
#define MARIO_RACCOON_ANI_WALK_RIGHT				25
#define MARIO_RACCOON_ANI_RUNNING_RIGHT				26
#define MARIO_RACCOON_ANI_JUMP_RIGHT				27
#define MARIO_RACCOON_ANI_STOP_RIGHT				28
#define MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_RIGHT		29
#define MARIO_RACCOON_ANI_SITTING_RIGHT				30
#define MARIO_RACCOON_ANI_FALLING_RIGHT				31
#define MARIO_RACCOON_ANI_FALLING_WAG_TAIL_RIGHT	32
#define MARIO_RACCOON_ANI_FLYING_RIGHT				33

#define MARIO_RACCOON_ANI_IDLE_LEFT					34
#define MARIO_RACCOON_ANI_WALK_LEFT					35
#define MARIO_RACCOON_ANI_RUNNING_LEFT				36
#define MARIO_RACCOON_ANI_JUMP_LEFT					37
#define MARIO_RACCOON_ANI_STOP_LEFT					38
#define MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_LEFT		39			
#define MARIO_RACCOON_ANI_SITTING_LEFT				40
#define MARIO_RACCOON_ANI_FALLING_LEFT				41
#define MARIO_RACCOON_ANI_FALLING_WAG_TAIL_LEFT		42
#define MARIO_RACCOON_ANI_FLYING_LEFT				43

#define MARIO_FIRE_ANI_IDLE_RIGHT							44
#define MARIO_FIRE_ANI_WALK_RIGHT							45
#define MARIO_FIRE_ANI_RUNNING_RIGHT						46
#define MARIO_FIRE_ANI_JUMP_RIGHT							47
#define MARIO_FIRE_ANI_STOP_RIGHT							48
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_RIGHT					49
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_RIGHT	50
#define MARIO_FIRE_ANI_SITTING_RIGHT						51
#define MARIO_FIRE_ANI_FALLING_RIGHT						52

#define MARIO_FIRE_ANI_IDLE_LEFT							53
#define MARIO_FIRE_ANI_WALK_LEFT							54
#define MARIO_FIRE_ANI_RUNNING_LEFT							55
#define MARIO_FIRE_ANI_JUMP_LEFT							56
#define MARIO_FIRE_ANI_STOP_LEFT							57
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_LEFT					58
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_LEFT	59
#define MARIO_FIRE_ANI_SITTING_LEFT							60
#define MARIO_FIRE_ANI_FALLING_LEFT							61

#define MARIO_ANI_DIE		62

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_RACCOON		3
#define MARIO_FIRE			4

#define MARIO_BIG_BBOX_WIDTH		15
#define MARIO_BIG_BBOX_WIDTH_RIGHT	20
#define MARIO_BIG_BBOX_HEIGHT		27
#define MARIO_BIG_BBOX_L			5

#define MARIO_SMALL_BBOX_WIDTH  15
#define MARIO_SMALL_BBOX_HEIGHT 16

#define MARIO_RACCOON_BBOX_WIDTH 15
#define MARIO_RACCOON_BBOX_HEIGHT 28
#define MARIO_RACCOON_BBOX_SIT	9
#define MARIO_RACCOON_BBOX_WIDTH_RIGHT	15
#define MARIO_RACCOON_BBOX_LEFT 7

#define MARIO_FIRE_BBOX_WIDTH	15
#define MARIO_FIRE_BBOX_HEIGHT	27

#define MARIO_BIG_SIT_BBOX_HEIGHT		18
#define MARIO_RACCOON_SIT_BBOX_HEIGHT	19
#define MARIO_FIRE_SIT_BBOX_HEIGHT		18
#define MARIO_SIT_BBOX					9

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	DWORD jump_start = 0;
	//bool isJumping;
	bool isRunning, isFalling, isSitting, isWaggingTail, canFly;
	bool isOnGround;
	bool specialAniCase;
	bool turnOn;
	int last_nx;
	int last_ani = -1;
	float last_y = INITIAL_LAST_Y;
	float last_vx;
	bool isWaitingForAni;

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
	bool IsSpecialAniCase();

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
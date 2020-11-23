﻿#pragma once
// Tileset
#define TILESET_BBOX_SIDE_LENGTH	16

// Brick
#define BRICK_BBOX_SIDE_LENGTH	16

// Brick contains item
#define BRICK_DEFLECT_SPEED_Y		0.1f
#define BRICK_BOUNCE_UP_TO			10

#define QUESTION_BRICK_ANI			0
#define NORMAL_BRICK_ANI			1
#define	DISGUISED_BRONZE_BRICK_ANI	2
#define	STATE_RAMMED				101
#define STATE_BEING_HIT_BY_TAIL		102

#define BRICK_VERTICAL_SEPARATION_LINE	352

// Bronze brick and Coin
#define BRONZE_BRICK_ANI		0
#define IDLE_COIN_ANI			1
#define ROTATING_COIN			2
#define TRANSFORMATION_TIME		7000 //8000
#define STATE_TRANSFORMATION	100
#define STATE_NORMAL			200

#define BRONZE_BRICK_HORIZONTAL_SEPARATION_LINE	367

enum BrickTransformation
{
	BRICK_FORM = 1,
	COIN_FORM
};

enum ItemOfBrick
{
	ITEM_RANDOM = 0,
	ITEM_MONEY = 1,
	//ITEM_SUPER_MUSHROOM = 2,
	//ITEM_SUPER_LEAF = 3,
	//ITEM_FIRE_FLOWER = 4,
	ITEM_UP_MUSHROOM = 5,
	ITEM_P_SWITCH = 6,
};

#define STATE_PRESSED				250
#define P_SWITCH_BBBOX_SIDE_LENGTH	16
#define P_SWITCH_NORMAL_ANI			0
#define P_SWITCH_PRESSED_ANI		1
#define P_SWITCH_POS_X				2032
#define P_SWITCH_POS_Y				352
	
#define SUPER_LEAF_ANI_LEFT						0
#define SUPER_LEAF_ANI_RIGHT					1
#define SUPER_LEAF_BBOX_WIDTH					16
#define SUPER_LEAF_BBOX_HEIGHT					14
#define SUPER_LEAF_DEFLECT_SPEED_Y				1.0f
#define SUPER_LEAF_SPEED_Y						0.35f
#define SUPER_LEAF_POSITION_ADJUSTMENT_NUM_Y	7
#define SUPER_LEAF_MAX_DISTANCE_FROM_BRICK		32
#define SUPER_LEAF_RIGHT_LIMIT_FROM_BRICK		32
#define SUPER_LEAF_SPEED_X_FACTOR				0.3 //
#define SUPER_LEAF_NUM_TO_THE_POWER_OF_SPEED_Y	35	//

#define SUPER_MUSHROOM_BBOX_WIDTH		16
#define SUPER_MUSHROOM_BBOX_HEIGHT		16
#define SUPER_MUSHROOM_DEFLECT_SPEED_Y	0.03f
#define SUPER_MUSHROOM_SPEED_Y			0.25f
#define SUPER_MUSHROOM_SPEED_X			0.06f
#define SUPER_MUSHROOM_ANI				0
#define UP_MUSHROOM_ANI					1

#define ICE_FLOWER_BBOX_WIDTH			16
#define ICE_FLOWER_BBOX_HEIGHT			16
#define ICE_FLOWER_DEFLECT_SPEED_Y		0.3f
#define ICE_FLOWER_MAX_EXISTING_TIME	2500

enum TypeOfBrickContainsItem
{
	QUESTION_BRICK = 1,
	DISGUISED_BRONZE_BRICK = 2,
};

// Pipe
#define PIPE_WIDTH			32
#define SHORT_PIPE_HEIGHT	32
#define MEDIUM_PIPE_HEIGHT	48
#define LONG_PIPE_HEIGHT	208

#define SHORT_PIPE_ANI		0
#define MEDIUM_PIPE_ANI		1
#define LONG_PIPE_ANI		2 

enum TypeOfPipe
{
	SHORT_PIPE = 1,
	MEDIUM_PIPE = 2,
	LONG_PIPE = 3,
};

// Effect
#define STATE_DESTROYED	99

#define HIT_EFFECT_TIME			150

#define	TAIL_HIT_EFFECT_TIME	100
#define	TAIL_HIT_EFFECT_WIDTH	16
#define	TAIL_HIT_EFFECT_HEIGHT	16

#define BROKEN_BRICK_PIECE_GRAVITY			0.0009f
#define BROKEN_BRICK_PIECE_WIDTH			8
#define	BROKEN_BRICK_PIECE_HEIGHT			8
#define	BROKEN_BRICK_PIECE_DEFLECT_SPEED_X	0.07f
#define	BROKEN_BRICK_PIECE_DEFLECT_SPEED_Y	0.16f

#define MONEY_EFFECT_100	0
#define MONEY_EFFECT_200	1
#define MONEY_EFFECT_400	2
#define MONEY_EFFECT_800	3
#define MONEY_EFFECT_1000	4
#define MONEY_EFFECT_2000	5
#define MONEY_EFFECT_4000	6
#define MONEY_EFFECT_8000	7

#define COIN_EFFECT_TIME					1300
#define COIN_EFFECT_SPEED_Y					0.14f
#define COIN_EFFECT_X_ADDEND				4
#define COIN_EFFECT_Y_SUBTRAHEND			14
#define COIN_EFFECT_MIN_Y_SUBTRAHEND		65
#define COIN_EFFECT_MAX_Y_SUBTRAHEND		20

#define MONEY_EFFECT_X_AT_BRICK_ADDEND		3
#define MONEY_EFFECT_Y_AT_BRICK_SUBTRAHEND	3
#define MONEY_EFFECT_SPEED_Y				0.04f

// Weapon
#define FIREBALL_BBOX_WIDTH		8
#define FIREBALL_BBOX_HEIGHT	8
#define FIREBALL_SPEED			0.15f
#define FIREBALL_TO_RIGHT		0
#define FIREBALL_TO_LEFT		1

#define TAIL_BBOX_WIDTH			9
#define TAIL_BBOX_HEIGHT		6

// Mario
#define MARIO_RUNNING_SPEED						0.23f
#define MARIO_MAX_WALKING_SPEED					0.12f
#define MARIO_RUNNING_ACCELERATION				0.00015f
#define MARIO_WALKING_ACCELERATION				0.00015f
#define MARIO_SMALL_ACCELERATION_SUBTRAHEND		0.00025f
#define MARIO_LARGE_ACCELERATION_SUBTRAHEND		0.0005f
#define INITIAL_LAST_Y							134.0f
#define MARIO_TOO_HIGH_ABOVE					150

#define	MARIO_WAGGING_TAIL_TIME			150
#define	MARIO_SPINNING_TAIL_TIME		375
#define	MARIO_SHOOTING_FIREBALL_TIME	300
#define MARIO_FLYING_TIME				4000
#define MARIO_KICK_TIME					200

#define MARIO_HIGH_JUMP_SPEED_Y		0.275f
#define MARIO_LOW_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_GRAVITY				0.0006f
#define MARIO_LOW_JUMP_GRAVITY		0.001f
#define MARIO_FLY_GRAVITY			0.00045f
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

#define MARIO_KICK							1111
#define MARIO_WAG_TAIL_WHILE_FALLING		1112
#define MARIO_WALKING_WHILE_HOLDING_SHELL	1113
#define MARIO_IDLE_WHILE_HOLDING_SHELL		1114
#define MARIO_ON_AIR_WHILE_HOLDING_SHELL	1115

#define MARIO_STATE_DIE				999

#define MARIO_ANI_BIG_IDLE_RIGHT				0
#define MARIO_ANI_BIG_WALKING_RIGHT				1
#define MARIO_ANI_BIG_RUNNING_RIGHT				2
#define MARIO_ANI_BIG_JUMP_RIGHT				3
#define MARIO_ANI_BIG_SITTING_RIGHT				4
#define MARIO_ANI_BIG_STOP_RIGHT				5
#define MARIO_ANI_BIG_FALLING_RIGHT				6
#define MARIO_ANI_BIG_KICK_RIGHT				67
#define	MARIO_ANI_BIG_IDLE_HOLD_SHELL_RIGHT		75
#define	MARIO_ANI_BIG_WALKING_HOLD_SHELL_RIGHT	76
#define	MARIO_ANI_BIG_ON_AIR_HOLD_SHELL_RIGHT	77

#define MARIO_ANI_BIG_IDLE_LEFT					7
#define MARIO_ANI_BIG_WALKING_LEFT				8
#define MARIO_ANI_BIG_RUNNING_LEFT				9
#define MARIO_ANI_BIG_JUMP_LEFT					10
#define MARIO_ANI_BIG_SITTING_LEFT				11
#define MARIO_ANI_BIG_STOP_LEFT					12
#define MARIO_ANI_BIG_FALLING_LEFT				13
#define MARIO_ANI_BIG_KICK_LEFT					68
#define	MARIO_ANI_BIG_IDLE_HOLD_SHELL_LEFT		78
#define	MARIO_ANI_BIG_WALKING_HOLD_SHELL_LEFT	79
#define	MARIO_ANI_BIG_ON_AIR_HOLD_SHELL_LEFT	80

#define MARIO_ANI_SMALL_IDLE_RIGHT					14
#define MARIO_ANI_SMALL_WALKING_RIGHT				15
#define MARIO_ANI_SMALL_RUNNING_RIGHT				16
#define MARIO_ANI_SMALL_JUMP_RIGHT					17
#define MARIO_ANI_SMALL_STOP_RIGHT					18
#define MARIO_ANI_SMALL_KICK_RIGHT					69
#define MARIO_ANI_SMALL_IDLE_HOLD_SHELL_RIGHT		81
#define MARIO_ANI_SMALL_WALKING_HOLD_SHELL_RIGHT	82
#define MARIO_ANI_SMALL_ON_AIR_HOLD_SHELL_RIGHT		83

#define MARIO_ANI_SMALL_IDLE_LEFT					19
#define MARIO_ANI_SMALL_WALKING_LEFT				20
#define MARIO_ANI_SMALL_RUNNING_LEFT				21
#define MARIO_ANI_SMALL_JUMP_LEFT					22
#define MARIO_ANI_SMALL_STOP_LEFT					23
#define MARIO_ANI_SMALL_KICK_LEFT					70
#define MARIO_ANI_SMALL_IDLE_HOLD_SHELL_LEFT		84
#define MARIO_ANI_SMALL_WALKING_HOLD_SHELL_LEFT		85
#define MARIO_ANI_SMALL_ON_AIR_HOLD_SHELL_LEFT		86

#define MARIO_RACCOON_ANI_IDLE_RIGHT					24
#define MARIO_RACCOON_ANI_WALK_RIGHT					25
#define MARIO_RACCOON_ANI_RUNNING_RIGHT					26
#define MARIO_RACCOON_ANI_JUMP_RIGHT					27
#define MARIO_RACCOON_ANI_STOP_RIGHT					28
#define MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_RIGHT			29
#define MARIO_RACCOON_ANI_SITTING_RIGHT					30
#define MARIO_RACCOON_ANI_FALLING_RIGHT					31
#define MARIO_RACCOON_ANI_FALLING_WAG_TAIL_RIGHT		32
#define MARIO_RACCOON_ANI_FLYING_UP_RIGHT				33
#define MARIO_RACCOON_ANI_FLYING_DOWN_RIGHT				63
#define MARIO_RACCOON_ANI_WAG_TAIL_WHILE_FLYING_RIGHT	64
#define MARIO_RACCOON_ANI_KICK_RIGHT					71
#define MARIO_RACCOON_ANI_IDLE_HOLD_SHELL_RIGHT			87
#define MARIO_RACCOON_ANI_WALKING_HOLD_SHELL_RIGHT		88
#define MARIO_RACCOON_ANI_ON_AIR_HOLD_SHELL_RIGHT		89

#define MARIO_RACCOON_ANI_IDLE_LEFT						34
#define MARIO_RACCOON_ANI_WALK_LEFT						35
#define MARIO_RACCOON_ANI_RUNNING_LEFT					36
#define MARIO_RACCOON_ANI_JUMP_LEFT						37
#define MARIO_RACCOON_ANI_STOP_LEFT						38
#define MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_LEFT			39			
#define MARIO_RACCOON_ANI_SITTING_LEFT					40
#define MARIO_RACCOON_ANI_FALLING_LEFT					41
#define MARIO_RACCOON_ANI_FALLING_WAG_TAIL_LEFT			42
#define MARIO_RACCOON_ANI_FLYING_UP_LEFT				43
#define MARIO_RACCOON_ANI_FLYING_DOWN_LEFT				65
#define MARIO_RACCOON_ANI_WAG_TAIL_WHILE_FLYING_LEFT	66
#define MARIO_RACCOON_ANI_KICK_LEFT						72
#define MARIO_RACCOON_ANI_IDLE_HOLD_SHELL_LEFT			90
#define MARIO_RACCOON_ANI_WALKING_HOLD_SHELL_LEFT		91
#define MARIO_RACCOON_ANI_ON_AIR_HOLD_SHELL_LEFT		92

#define MARIO_FIRE_ANI_IDLE_RIGHT							44
#define MARIO_FIRE_ANI_WALK_RIGHT							45
#define MARIO_FIRE_ANI_RUNNING_RIGHT						46
#define MARIO_FIRE_ANI_JUMP_RIGHT							47
#define MARIO_FIRE_ANI_STOP_RIGHT							48
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_RIGHT					49
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_RIGHT	50
#define MARIO_FIRE_ANI_SITTING_RIGHT						51
#define MARIO_FIRE_ANI_FALLING_RIGHT						52
#define MARIO_FIRE_ANI_KICK_RIGHT							73
#define MARIO_FIRE_ANI_IDLE_HOLD_SHELL_RIGHT				93
#define MARIO_FIRE_ANI_WALKING_HOLD_SHELL_RIGHT				94
#define MARIO_FIRE_ANI_ON_AIR_HOLD_SHELL_RIGHT				95

#define MARIO_FIRE_ANI_IDLE_LEFT							53
#define MARIO_FIRE_ANI_WALK_LEFT							54
#define MARIO_FIRE_ANI_RUNNING_LEFT							55
#define MARIO_FIRE_ANI_JUMP_LEFT							56
#define MARIO_FIRE_ANI_STOP_LEFT							57
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_LEFT					58
#define MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_LEFT	59
#define MARIO_FIRE_ANI_SITTING_LEFT							60
#define MARIO_FIRE_ANI_FALLING_LEFT							61
#define MARIO_FIRE_ANI_KICK_LEFT							74
#define MARIO_FIRE_ANI_IDLE_HOLD_SHELL_LEFT					96
#define MARIO_FIRE_ANI_WALKING_HOLD_SHELL_LEFT				97
#define MARIO_FIRE_ANI_ON_AIR_HOLD_SHELL_LEFT				98

#define MARIO_ANI_DIE		62

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_RACCOON		3
#define MARIO_FIRE			4

#define MARIO_BBOX_HEIGHT				30
#define MARIO_SIT_BBOX_DIFFERENCE_NUM	12

#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_TOP_ADDEND			3
#define MARIO_BIG_LEFT_SUBTRAHEND_R		10
#define MARIO_BIG_LEFT_SUBTRAHEND_L		9

#define MARIO_SMALL_BBOX_WIDTH			14
#define MARIO_SMALL_TOP_ADDEND			14
#define MARIO_SMALL_LEFT_SUBTRAHEND_R	11
#define MARIO_SMALL_LEFT_SUBTRAHEND_L	9

#define MARIO_RACCOON_BBOX_WIDTH		15
#define MARIO_RACCOON_TOP_ADDEND		2
#define MARIO_RACCOON_LEFT_SUBTRAHEND_R	10
#define MARIO_RACCOON_LEFT_SUBTRAHEND_L	8

#define MARIO_UNTOUCHABLE_TIME 2000


// ENEMIES
// cho enemies xài chung deflect speed x và y?
#define ENEMY_STATE_ATTACKED_BY_TAIL			188  // lead to die or sleep
#define ENEMY_STATE_DIE_BY_WEAPON				199
#define ENEMY_STATE_MOVE						200
#define ENEMY_STATE_IDLE						201
#define ENEMY_DEFECT_SPEED_X_CAUSED_BY_TAIL		0.035f
#define ENEMY_DEFECT_SPEED_Y_CAUSED_BY_TAIL		0.28f

// Goomba
#define GOOMBA_MOVE_SPEED_X						0.035f
#define GOOMBA_DEFLECT_SPEED_Y					0.15f
#define GOOMBA_DEFLECT_SPEED_X					0.03f
#define GOOMBA_BBOX_WIDTH						16
#define GOOMBA_BBOX_HEIGHT						16
#define GOOMBA_BBOX_HEIGHT_DIE_BY_CRUSH			9
#define GOOMBA_ANI_MOVE							0
#define GOOMBA_ANI_DIE_BY_CRUSH					1
#define GOOMBA_ANI_DIE_BY_ATTACK_TOOL			2
#define GOOMBA_STATE_DIE_BY_CRUSH				299
#define GOOMBA_MAX_EXISTING_TIME_AFTER_DEATH	250

// Koopa
enum PositionOfKoopa
{
	ON_GREEN_COLOR_BOX = 1,
	ON_PURPLE_COLOR_BOX = 2,
	ON_BRONZE_BRICK = 3,
	ON_GROUND = 4,
};

#define GCB_KOOPA_POS_X	591
#define GCB_KOOPA_POS_Y	356
#define PCB_KOOPA_POS_X
#define PCB_KOOPA_POS_Y
#define BB_KOOPA_POS_X
#define BB_KOOPA_POS_Y
#define G_KOOPA_POS_X
#define G_KOOPA_POS_Y

#define KOOPA_MOVE_SPEED_X					0.025f
#define KOOPA_SPIN_AND_MOVE_SPEED_X			0.2f
#define KOOPA_DEFLECT_SPEED_Y				0.15f
#define KOOPA_DEFLECT_SPEED_X				0.03f
#define KOOPA_LEFT_ADDEND					1
#define KOOPA_BBOX_WIDTH					16
#define KOOPA_BBOX_HEIGHT					27
#define KOOPA_BBOX_HEIGHT_LAY_VIBRATE_SPIN	16
#define KOOPA_BBOX_WIDTH_VIBRATE			18
#define KOOPA_ANI_MOVE_RIGHT				0
#define KOOPA_ANI_MOVE_LEFT					1
#define KOOPA_ANI_LAY_PRONE					2
#define KOOPA_ANI_LAY_SUPINE				3
#define KOOPA_ANI_VIBRATE_PRONE				4
#define KOOPA_ANI_VIBRATE_SUPINE			5
#define KOOPA_ANI_SPIN_AND_MOVE_PRONE		6
#define KOOPA_ANI_SPIN_AND_MOVE_SUPINE		7
#define KOOPA_STATE_SPIN_AND_MOVE			80
#define KOOPA_STATE_BEING_HELD				81

// Fire Piranha
enum TypeOfFirePiranha
{
	RED = 1,
	GREEN = 2,
};

#define	FIRE_PIRANHA_STATE_MOVE_UP			10
#define	FIRE_PIRANHA_STATE_MOVE_DOWN		11
#define	FIRE_PIRANHA_STATE_ATTACK			12
#define FIRE_PIRANHA_MOVE_SPEED_Y			0.06f
#define FIRE_PIRANHA_DELAY_TIME				1500
#define	FIRE_PIRANHA_DELAY_TO_ATTACK_TIME	700
#define FIRE_PIRANHA_MAX_Y					369

#define FIRE_PIRANHA_ANI_FACE_UP_LEFT		0
#define FIRE_PIRANHA_ANI_FACE_DOWN_LEFT		1
#define FIRE_PIRANHA_ANI_FACE_UP_RIGHT		2
#define FIRE_PIRANHA_ANI_FACE_DOWN_RIGHT	3
#define FIRE_PIRANHA_ANI_ATTACK_UP_LEFT		4
#define FIRE_PIRANHA_ANI_ATTACK_DOWN_LEFT	5
#define FIRE_PIRANHA_ANI_ATTACK_UP_RIGHT	6
#define FIRE_PIRANHA_ANI_ATTACK_DOWN_RIGHT	7

#define RED_FIRE_PIRANHA_MIN_Y				336
#define RED_FIRE_PIRANHA_BBOX_WIDTH			16
#define RED_FIRE_PIRANHA_BBOX_HEIGHT		32
#define RED_FIRE_PIRANHA_FAR_LEFT_START		191
#define RED_FIRE_PIRANHA_NEAR_LEFT_START	296	
#define RED_FIRE_PIRANHA_NEAR_RIGHT_START	368	
#define RED_FIRE_PIRANHA_FAR_RIGHT_START	440	
#define RED_FIRE_PIRANHA_FAR_RIGHT_END		535
#define RED_FIRE_PIRANHA_SAFE_ZONE_LEFT		348
#define RED_FIRE_PIRANHA_SAFE_ZONE_RIGHT	387
#define RED_FIRE_PIRANHA_SAFE_ZONE_BOTTOM	415

#define GREEN_FIRE_PIRANHA_MIN_Y			344
#define GREEN_FIRE_PIRANHA_BBOX_WIDTH		16
#define GREEN_FIRE_PIRANHA_BBOX_HEIGHT		24
#define GREEN_FIRE_PIRANHA_FAR_LEFT_START	1728
#define GREEN_FIRE_PIRANHA_NEAR_LEFT_START	1802
#define GREEN_FIRE_PIRANHA_NEAR_RIGHT_START	1872
#define GREEN_FIRE_PIRANHA_FAR_RIGHT_START	1956	
#define GREEN_FIRE_PIRANHA_FAR_RIGHT_END	2012
#define GREEN_FIRE_PIRANHA_SAFE_ZONE_LEFT	1852
#define GREEN_FIRE_PIRANHA_SAFE_ZONE_RIGHT	1891
#define GREEN_FIRE_PIRANHA_SAFE_ZONE_BOTTOM	415

#define PIRANHA_MAX_EXISTING_TIME_AFTER_DEATH	500
#define PIRANHA_FIREBALL_SPEED_X				0.05f

enum Area
{
	TOP_LEFT_FAR,
	TOP_LEFT_NEAR,
	TOP_RIGHT_FAR,
	TOP_RIGHT_NEAR,
	BOTTOM_LEFT_FAR,
	BOTTOM_LEFT_NEAR,
	BOTTOM_RIGHT_FAR,
	BOTTOM_RIGHT_NEAR,
	OUTSIDE_AREA,
};

// Green Piranha
#define	GREEN_PIRANHA_STATE_MOVE_UP			10
#define	GREEN_PIRANHA_STATE_MOVE_DOWN		11
#define GREEN_PIRANHA_BBOX_WIDTH			16
#define GREEN_PIRANHA_BBOX_HEIGHT			24
#define GREEN_PIRANHA_MOVE_SPEED_Y			0.06f
#define GREEN_PIRANHA_DELAY_TIME			820
#define GREEN_PIRANHA_MAX_Y					385
#define GREEN_PIRANHA_MIN_Y					361
#define GREEN_PIRANHA_SAFE_ZONE_LEFT		1788
#define GREEN_PIRANHA_SAFE_ZONE_RIGHT		1827
#define GREEN_PIRANHA_SAFE_ZONE_BOTTOM		415


// PlayScene
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TILEMAP			7

#define MAX_SCENE_LINE 1024

// list of object type
enum Type
{
	PORTAL = 55,
	MARIO = 0,
	BRICK_CONTAINS_ITEM = 1,
	GOOMBA = 2,
	KOOPA = 3,
	FLOOR = 4,
	COLOR_BOX = 5,
	FIRE_PIRANHA = 6,
	GREEN_PIRANHA = 7,
	HAMMER = 8,
	FIREBALL = 9,
	TAIL = 10,
	BRONZE_BRICK = 11,
	PIPE = 12,
	SUPER_LEAF = 13,
	SUPER_MUSHROOM = 14,
	ICE_FLOWER = 15,
	COIN = 16,
	P_SWITCH = 17,
};

enum Category
{
	PLAYER,
	ENEMY,
	MISC,
	ITEM,
	WEAPON,
	EFFECT,
	SCENE,
};